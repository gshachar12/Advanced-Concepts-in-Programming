@echo off
REM Tank Battle Game - Quick Run Script
REM Project 3 - Advanced Concepts in Programming

echo.
echo ============================================
echo    🎮 Tank Battle Game - Project 3
echo ============================================
echo.

if "%~1"=="" (
    echo Usage: run.bat [command] [args...]
    echo.
    echo Available commands:
    echo   build           - Build the game
    echo   demo            - Run demonstration game
    echo   list            - List available algorithms  
    echo   tournament      - Run full tournament
    echo   quick-tour      - Run quick tournament
    echo   simple-vs-bfs   - Simple vs BFS battle
    echo   bfs-vs-simple   - BFS vs Simple battle
    echo   custom          - Run with custom parameters
    echo   help            - Show detailed help
    echo.
    echo Examples:
    echo   run.bat demo
    echo   run.bat tournament  
    echo   run.bat custom -map inputs\maze_map.txt -algo1 BFS -algo2 Simple -verbose
    echo.
    goto :end
)

if "%~1"=="build" (
    echo 🔨 Building Tank Battle Game...
    mingw32-make clean
    mingw32-make all
    goto :end
)

if "%~1"=="demo" (
    echo 🎯 Running Demo Game...
    mingw32-make demo
    goto :end
)

if "%~1"=="list" (
    echo 📋 Listing Available Algorithms...
    mingw32-make list
    goto :end
)

if "%~1"=="tournament" (
    echo 🏆 Running Full Tournament...
    mingw32-make tournament
    goto :end
)

if "%~1"=="quick-tour" (
    echo 🏆 Running Quick Tournament...
    mingw32-make quick-tournament
    goto :end
)

if "%~1"=="simple-vs-bfs" (
    echo ⚔️ Simple vs BFS Battle...
    mingw32-make simple-vs-bfs
    goto :end
)

if "%~1"=="bfs-vs-simple" (
    echo ⚔️ BFS vs Simple Battle...
    mingw32-make bfs-vs-simple
    goto :end
)

if "%~1"=="custom" (
    echo 🎮 Running Custom Game...
    shift
    tanks_game.exe %*
    goto :end
)

if "%~1"=="help" (
    echo 🎮 Tank Battle Game - Project 3
    echo ═══════════════════════════════════
    echo.
    echo This is a tank battle simulation game with plugin architecture.
    echo Tanks battle on a grid-based map using different AI algorithms.
    echo.
    echo Command Line Arguments:
    echo   -map ^<file^>         Map file to load
    echo   -steps ^<num^>        Maximum steps per game ^(default: 100^)
    echo   -shells ^<num^>       Shells per tank ^(default: 5^)
    echo   -algo1 ^<name^>       Algorithm for player 1
    echo   -algo2 ^<name^>       Algorithm for player 2  
    echo   -verbose            Enable verbose output
    echo   -tournament         Run tournament mode
    echo   -games ^<num^>        Games per tournament matchup
    echo   -list               List available algorithms
    echo   -help               Show help message
    echo.
    echo Available Maps:
    echo   inputs\simple_map.txt   - Basic 10x9 map
    echo   inputs\maze_map.txt     - Complex maze layout
    echo   inputs\open_field.txt   - Large open battlefield
    echo.
    echo Available Algorithms:
    tanks_game.exe -list
    goto :end
)

echo ❌ Unknown command: %~1
echo Use 'run.bat help' for detailed usage information.

:end
