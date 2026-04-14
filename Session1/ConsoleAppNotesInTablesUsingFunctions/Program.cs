Console.WriteLine("Exercice 3");
Console.ForegroundColor = ConsoleColor.Blue;
Console.WriteLine("Appuyez sur Entrée pour quitter sans saisir de valeur.");
Console.ResetColor();

// Validation de la taille du tableau saisi par l'utilisateur
var size = GetTableSizeFromUser();

// Initialisation du tableau
var choice = GetTableFillChoiceFromUser();

var table = new int[size];

// Remplissage du tableau selon le choix de l'utilisateur
switch (choice)
{
    case "1": // Remplissage du tableau manuellement par l'utilisateur
        FillTableManually(table);
        break;
    case "2": // Remplissage du tableau automatiquement avec des valeurs aléatoires entre 0 et 20
        FillTableAutomatically(table);
        break;
    default: // Choix invalide
        Console.ForegroundColor = ConsoleColor.Red;
        Console.WriteLine("Choix invalide. Veuillez appuyer sur Entrée pour quitter.");
        Console.ResetColor();
        break;
}

// Affichage du tableau final
DisplayTable(table, out var tableTotal);

Console.ForegroundColor = ConsoleColor.Green;

// Calcul et affichage de la moyenne du tableau
var average = tableTotal / size;
Console.WriteLine("Moyenne du tableau: " + average);

// Affichage des notes supérieures à la moyenne
DisplayAboveAverageNotes(table, average);

Console.ResetColor();

// Fonctions utilisées dans le programme
static int GetTableSizeFromUser()
{
    var size = 0;
    while (size <= 0)
    {
        Console.Write("Veuillez entrer la taille du tableau: ");
        var sizeStr = Console.ReadLine();
        if (string.IsNullOrEmpty(sizeStr))
        {
            Environment.Exit(0);
        }
        try
        {
            size = Convert.ToInt16(sizeStr);
            if (size <= 0)
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine("Veuillez entrer un nombre entier positif pour la taille du tableau.");
                Console.ResetColor();
            }
        }
        catch (FormatException)
        {
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine("Veuillez entrer un nombre entier valide pour la taille du tableau.");
            Console.ResetColor();
        }
    }
    return size;
}

static string GetTableFillChoiceFromUser()
{
    Console.WriteLine("Comment souhaitez-vous remplir le tableau ?");
    Console.WriteLine("1. Manuellement");
    Console.WriteLine("2. Automatiquement avec des valeurs aléatoires entre 0 et 20");
    Console.Write("Veuillez entrer votre choix (1 ou 2): ");
    var choice = Console.ReadLine();
    if (string.IsNullOrEmpty(choice))
    {
        Environment.Exit(0);
    }
    return choice;
}

static void FillTableManually(int[] table)
{
    for (var i = 0; i < table.Length; i++)
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
}

static void FillTableAutomatically(int[] table)
{
    var random = new Random();
    for (var i = 0; i < table.Length; i++)
    {
        table[i] = random.Next(0, 21); // Génère un nombre aléatoire entre 0 et 20
    }
}

static void DisplayTable(int[] table, out int tableTotal)
{
    tableTotal = 0;
    var size = table.Length;
    var tableValue = string.Empty;
    for (var i = 0; i < table.Length; i++)
    {
        tableValue += table[i] + (i == size - 1 ? "" : ", ");
        tableTotal += table[i];
    }
    Console.WriteLine($"Contenu du tableau: {tableValue}");
}

static void DisplayAboveAverageNotes(int[] table, double average)
{
    var tableNotesHigherThanAverage = string.Empty;
    var count = 0;
    for (var i = 0; i < table.Length; i++)
    {
        if (table[i] > average)
        {
            tableNotesHigherThanAverage += table[i] + ", ";
            count++;
        }
    }
    // Suppression de la virgule et de l'espace à la fin de la chaîne si elle n'est pas vide
    tableNotesHigherThanAverage = tableNotesHigherThanAverage.TrimEnd(',', ' ');

    Console.WriteLine($"Notes supérieures à la moyenne ({count}): {tableNotesHigherThanAverage}");
}