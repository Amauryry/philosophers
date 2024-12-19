#!/bin/bash

# Nom du fichier de sortie
output_file="eating_counts.txt"

# Nettoyer le fichier de sortie si existant
> "$output_file"

# Nombre de fois que le programme doit être lancé
iterations=500

# Commande à exécuter
philo_command="./philo 5 800 200 200 7"

for ((i=1; i<=iterations; i++))
do
    echo "Execution $i..."

    # Lancer le programme et compter les "eating"
    $philo_command > log.txt
    eating_count=$(grep -o "eating" log.txt | wc -l)

    # Ajouter le résultat au fichier de sortie
    echo "$eating_count" >> "$output_file"

done

echo "Tous les résultats ont été enregistrés dans $output_file."
