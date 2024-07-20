// See https://aka.ms/new-console-template for more information

using Wrapper;

var library = new CSharpTypeLibrary();
var context = library.Initialize();

Console.WriteLine("Hello, World!");

Action<int, float> callback = (x, y) =>
{
    Console.WriteLine(string.Format("int : {0} double : {1}", x, y));
};

library.RegisterCallback(context, callback);
library.RunCallback(context, 3, 4.0f);
library.UnregisterCallback(context);

library.Release(context);