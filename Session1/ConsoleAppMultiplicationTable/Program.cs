Console.WriteLine("Bienvenue dans cette application de simulation de tables de multiplication!");
Console.ForegroundColor = ConsoleColor.Blue;
Console.WriteLine("Appuyez sur Entrée pour quitter sans saisir de valeur.");
Console.ResetColor();
Console.Write("Veuillez entrer un nombre pour lequel vous souhaitez voir la table de multiplication: ");
var number = Console.ReadLine();
if (string.IsNullOrEmpty(number))
{
    Environment.Exit(0);
}

// Initilisation de l'intervalle
var start = 0;
var end = 0;

// Validation de l'intervalle saisi par l'utilisateur
while ((start == 0 && end == 0) || start > end)
{
    Console.Write("Veuillez entrer le nombre de début de l'intervalle: ");
    var startStr = Console.ReadLine();
    if (string.IsNullOrEmpty(startStr))
    {
        Environment.Exit(0);
    }
    try
    {
        start = Convert.ToInt16(startStr);
    }
    catch (FormatException)
    {
        Console.ForegroundColor = ConsoleColor.Red;
        Console.WriteLine("Veuillez entrer un nombre valide pour le début de l'intervalle.");
        Console.ResetColor();
        continue;
    }

    Console.Write("Veuillez entrer le nombre de fin de l'intervalle: ");
    var endStr = Console.ReadLine();
    if (string.IsNullOrEmpty(endStr))
    {
        Environment.Exit(0);
    }
    try
    {
        end = Convert.ToInt16(endStr);
    }
    catch (FormatException)
    {
        Console.ForegroundColor = ConsoleColor.Red;
        Console.WriteLine("Veuillez entrer un nombre valide pour la fin de l'intervalle.");
        Console.ResetColor();
        continue;
    }

    if (start > end)
    {
        Console.ForegroundColor = ConsoleColor.Red;
        Console.WriteLine("Le nombre de début de l'intervalle doit être inférieur ou égal au nombre de fin.");
        Console.ResetColor();
    }
}

// Affichage de la table de multiplication
Console.ForegroundColor = ConsoleColor.Green;
for (var i = start; i <= end; i++)
{
    Console.ForegroundColor = ConsoleColor.Green;
    Console.WriteLine($"{number} x {i} = {Convert.ToInt16(number) * i}");
}
Console.ResetColor();
