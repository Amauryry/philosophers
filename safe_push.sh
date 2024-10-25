#!/bin/bash

# Ajouter tous les fichiers modifiés
git add .
sleep 1
# Commit avec le message "safe push"
git commit -m "safe push"
sleep 1
# Pousser les modifications vers le dépôt distant
git push
sleep 1