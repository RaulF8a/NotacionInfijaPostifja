#include <iostream>
#include <stdlib.h>
#include "Pila.h"

using namespace std;

size_t precedencia (char opreador);
bool isOperador (char caracter);

int main (){
    system ("cls");

    //Declaracion de variables.
    string notacionAritmetica, notacionPostfija;
    Pila<char> operadores;

    cout << "\t\tNotacion Aritmetica a Postfija\n" << endl;

    cout << "Digita la operacion en notacion aritmetica: ";
    fflush (stdin);
    getline (cin, notacionAritmetica);

    for (size_t i = 0; i < notacionAritmetica.size(); i++){
        //Evaluamos si el caracter es un operando. Tiene que ser distinto a los operadores y a los parentesis.
        if (!isOperador(notacionAritmetica[i]) and (notacionAritmetica[i] != '(' and notacionAritmetica[i] != ')')){
            notacionPostfija += notacionAritmetica[i];
        }

        //Evaluamos si tenemos un parentesis izquierdo.
        else if (notacionAritmetica[i] == '('){
            //De ser verdadero, lo agregamos a la pila.
            operadores.push (notacionAritmetica[i]);
        }

        //Evaluamos si tenemos un parentesis derecho.
        else if (notacionAritmetica[i] == ')'){
            //De ser verdadero, vaciamos la pila hasta encontrar un parentesis izquierdo. Agregamos cada elemento a la salida.
            while (operadores.top () != '('){
                notacionPostfija += operadores.top ();
                operadores.pop ();
            }

            //Eliminamos el parentesis izquierdo que quedo, para dejar la cola vacia.
            operadores.pop ();
        }

        //Evaluamos si tenemos un operador.
        else if (isOperador(notacionAritmetica[i])){
            //Evaluamos la precedencia del operador actual con el operador al tope de la pila, y mientras sea mayor o igual, los agregamos
            //a la cadena de salida y los eliminamos.
            while (!operadores.empty () and (precedencia (operadores.top ()) >= precedencia (notacionAritmetica[i]))){
                notacionPostfija += operadores.top ();
                operadores.pop ();
            }

            //Agregamos nuestro nuevo operador a la pila.
            operadores.push (notacionAritmetica[i]);
        }
    }

    //Agregamos todos los elementos que hayan quedado en la pila a la salida.
    while (!operadores.empty ()){
        notacionPostfija += operadores.top();
        operadores.pop ();
    }

    //Imprimimos la notacion postfija.
    cout << "\nLa notacion postfija de tu expresion es: " << notacionPostfija << endl;

    cout << endl;

    system ("pause");
    return 0;
}

size_t precedencia (char operador){
    if (operador == '+' or operador == '-'){
        return 1;
    }
    else if (operador == '*' or operador == '/'){
        return 2;
    }
    else if (operador == '^'){
        return 3;
    }
    else{
        return 0;
    }
}

bool isOperador (char caracter){
    return precedencia (caracter) != 0;
}