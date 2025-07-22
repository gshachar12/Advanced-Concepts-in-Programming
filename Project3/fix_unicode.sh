#!/bin/bash
# Script to replace unicode emojis with ASCII text

# GameManager.cpp replacements
sed -i 's/❌/ERROR:/g' src/GameManager.cpp
sed -i 's/✅/OK:/g' src/GameManager.cpp
sed -i 's/🚀/SHELL:/g' src/GameManager.cpp
sed -i 's/🏆/ACTION:/g' src/GameManager.cpp
sed -i 's/🥇/WINNER:/g' src/GameManager.cpp
sed -i 's/🤝/TIE:/g' src/GameManager.cpp

# main.cpp replacements
sed -i 's/🎮/GAME:/g' src/main.cpp
sed -i 's/🏆/TOURNAMENT:/g' src/main.cpp
sed -i 's/⚔️/BATTLE:/g' src/main.cpp
sed -i 's/❌/ERROR:/g' src/main.cpp
sed -i 's/✅/OK:/g' src/main.cpp
sed -i 's/🚀/START:/g' src/main.cpp
sed -i 's/🥇/WINNER:/g' src/main.cpp
sed -i 's/🤝/TIE:/g' src/main.cpp
sed -i 's/📋/LIST:/g' src/main.cpp

# TournamentManager.cpp replacements
sed -i 's/🏆/TOURNAMENT:/g' src/algorithms/TournamentManager.cpp
sed -i 's/✅/OK:/g' src/algorithms/TournamentManager.cpp
sed -i 's/❌/ERROR:/g' src/algorithms/TournamentManager.cpp
sed -i 's/🤝/TIE:/g' src/algorithms/TournamentManager.cpp

# AlgorithmFactory.cpp replacements
sed -i 's/✅/OK:/g' src/algorithms/AlgorithmFactory.cpp
sed -i 's/📋/LIST:/g' src/algorithms/AlgorithmFactory.cpp

echo "Unicode characters replaced with ASCII equivalents"
