Console.WriteLine("Exercice 2");
Console.ForegroundColor = ConsoleColor.Blue;
Console.WriteLine("Appuyez sur Entrée pour quitter sans saisir de valeur.");
Console.ResetColor();

// Validation de la taille du tableau saisi par l'utilisateur
Console.Write("Veuillez entrer la taille du tableau: ");
var sizeStr = Console.ReadLine();
if (string.IsNullOrEmpty(sizeStr))
{
    Environment.Exit(0);
}
var size = 0;
while (size <= 0)
{
    try
    {
        size = Convert.ToInt32(sizeStr);
    }
    catch (FormatException)
    {
        Console.ForegroundColor = ConsoleColor.Red;
        Console.WriteLine("Veuillez entrer un nombre valide pour la taille du tableau.");
        Console.ResetColor();
        Environment.Exit(0);
    }
}

// Initialisation du tableau
Console.WriteLine("Remplissage des valeurs du tableau.");
var choice = "-1";
while (choice == "-1")
{
    // Choix de l'utilisateur pour remplir le tableau
    Console.WriteLine("Appuyez 1 pour saisir les valeur par vous même.");
    Console.WriteLine("Appuyez 2 pour remplir le tableau automatiquement.");
    Console.Write("Entrez votre choix: ");
    choice = Console.ReadLine();
    if (string.IsNullOrEmpty(choice))
    {
        Environment.Exit(0);
    }
}

var table = new int[size];

// Remplissage du tableau selon le choix de l'utilisateur
switch (choice) {
    case "1": // Remplissage du tableau manuellement par l'utilisateur
        
        for (var i = 0; i < size; i++)
        {
            Console.Write($"Veuillez entrer la valeur pour la position [{i}]: ");
            var valueStr = Console.ReadLine();
            if (string.IsNullOrEmpty(valueStr))
            {
                Environment.Exit(0);
            }
            try
            {
                table[i] = Convert.ToInt32(valueStr);
            }
            catch (FormatException)
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine($"Veuillez entrer un nombre valide pour la position [{i}].");
                Console.ResetColor();
                Environment.Exit(0);
            }
        }
        break;
    case "2": // Remplissage du tableau automatiquement avec des valeurs aléatoires entre 0 et 20
        Random rand = new();
        for (var i = 0; i < size; i++)
        {
            table[i] = rand.Next(0, 20);
        }
        break;
    default: // Choix invalide
        Console.ForegroundColor = ConsoleColor.Red;
        Console.WriteLine("Choix invalide. Veuillez appuyer sur Entrée pour quitter.");
        Console.ResetColor();
        break;
}

// Affichage du tableau final
var tableValue = string.Empty;
var tableTotal = 0;
for (var i = 0; i < size; i++)
{
    tableValue += table[i] + (i == size - 1 ? "" : ", ");
    tableTotal += table[i];
}
Console.WriteLine($"Tableau final: {tableValue}");

// Calcul et affichage de la moyenne du tableau
Console.ForegroundColor = ConsoleColor.Green;
var average = tableTotal / size;
Console.WriteLine("Moyenne du tableau: " + average);

// Affichage des notes supérieures à la moyenne
var tableNotesHigherThanAverage = string.Empty;
for (var i = 0; i < size; i++)
{
    tableNotesHigherThanAverage += table[i] > average 
        ? table[i] + (i == size - 1 ? "" : ", ") 
        : "";
}
// Suppression de la virgule et de l'espace à la fin de la chaîne si elle n'est pas vide
tableNotesHigherThanAverage = tableNotesHigherThanAverage.TrimEnd(',', ' ');

Console.WriteLine("Notes supérieures à la moyenne: " + tableNotesHigherThanAverage);
Console.ResetColor();