# build-quick.ps1: Quick development build script for MyCppLibraryProject
#
# Purpose:
#   - Fast build for development and testing
#   - Supports Debug/Release builds
#   - Configurable test building
#
# Usage: 
#   .\build-quick.ps1 [options]
#
# Options:
#   -Help         Show this help message
#   -WithTests    Build with tests (default)
#   -NoTests      Build without tests
#   -Clean        Clean build directory before building
#   -Verbose      Show verbose test output with individual test details

# Parse command line arguments
param(
    [switch]$Help,
    [switch]$WithTests,
    [switch]$NoTests,
    [switch]$Clean,
    [switch]$Verbose
)

# Exit on error
$ErrorActionPreference = "Stop"

# Default configuration
$BuildTests = "ON"
$CleanBuild = $false
$VerboseTests = $false

# Colors for output
$Red = "Red"
$Green = "Green"
$Yellow = "Yellow"
$Blue = "Cyan" # Using Cyan instead of Blue for better visibility in PowerShell

# Function to print section header
function Print-Section($text) {
    Write-Host ""
    Write-Host "=== $text ===" -ForegroundColor $Yellow
    Write-Host ""
}

# Function to print status
function Print-Status($text) {
    Write-Host $text -ForegroundColor $Blue
}

# Function to show help
function Show-Help {
    Write-Host "Usage: $($MyInvocation.MyCommand.Name) [options]"
    Write-Host ""
    Write-Host "Options:"
    Write-Host "  -Help         Show this help message"
    Write-Host "  -WithTests    Build with tests (default)"
    Write-Host "  -NoTests      Build without tests"
    Write-Host "  -Clean        Clean build directory before building"
    Write-Host "  -Verbose      Show verbose test output with individual test details"
    Write-Host ""
    Write-Host "Examples:"
    Write-Host "  .\build-quick.ps1             # Debug build with tests"
    Write-Host "  .\build-quick.ps1 -NoTests    # Debug build without tests"
    Write-Host "  .\build-quick.ps1 -Clean      # Clean debug build with tests"
    exit 0
}

if ($Help) {
    Show-Help
}

if ($WithTests) {
    $BuildTests = "ON"
}

if ($NoTests) {
    $BuildTests = "OFF"
}

if ($Clean) {
    $CleanBuild = $true
}

if ($Verbose) {
    $VerboseTests = $true
}

Print-Section "Quick Build Configuration"
Write-Host "Build Tests: " -NoNewline
Write-Host $BuildTests -ForegroundColor $Blue
Write-Host "Clean Build: " -NoNewline
Write-Host $CleanBuild -ForegroundColor $Blue
Write-Host "Verbose Tests: " -NoNewline
Write-Host $(if ($VerboseTests) { "ON" } else { "OFF" }) -ForegroundColor $Blue

# Clean if requested
if ($CleanBuild) {
    Print-Section "Cleaning Build Directory"
    if (Test-Path "build") {
        Remove-Item -Recurse -Force "build"
    }
}

# Create build directory
if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
}
Set-Location -Path "build"

# Configure
Print-Section "Configuring Build"
Print-Status "Running CMake..."

# Get number of processors for parallel build
$NumProcs = (Get-CimInstance Win32_ComputerSystem).NumberOfLogicalProcessors

# Run CMake
try {
    cmake -DCMAKE_BUILD_TYPE=Debug `
          -DBUILD_TESTING=$BuildTests `
          -DCMAKE_EXPORT_COMPILE_COMMANDS=ON `
          ..
} catch {
    Write-Host "CMake configuration failed." -ForegroundColor $Red
    Write-Host $_.Exception.Message -ForegroundColor $Red
    exit 1
}

# Build
Print-Section "Building Project"
Print-Status "Running build..."

# Use cmake --build instead of make for Windows compatibility
try {
    cmake --build . --config Debug --parallel $NumProcs
} catch {
    Write-Host "Build failed." -ForegroundColor $Red
    Write-Host $_.Exception.Message -ForegroundColor $Red
    exit 1
}

# Run tests if enabled
if ($BuildTests -eq "ON") {
    Print-Section "Running Tests"
    if ($VerboseTests) {
        Print-Status "Running CTest with verbose output..."
        ctest --output-on-failure -V -C Debug
    } else {
        Print-Status "Running CTest..."
        ctest --output-on-failure -C Debug
    }
}

# Copy compile commands to root for tooling if it exists
if (Test-Path "compile_commands.json") {
    Copy-Item -Path "compile_commands.json" -Destination ".."
}

# Return to original directory
Set-Location -Path ".."

Print-Section "Build Summary"
Write-Host "Build completed successfully!" -ForegroundColor $Green
