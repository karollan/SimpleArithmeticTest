#include<iostream>
#include<iomanip>
#include<ctime>
#include<sstream>
#include<stack>
#include<cstdlib>
#include<cmath>
using namespace std;


//Struktura poziomu zawiera dane jednego etapu
struct poziom 
{
	double a;
	double b;
	double c;
	double wynik;
	double popOdp;
	char op1;
	char op2;
	double czas;
	string trudnosc;
};

//Struktura zawiera dane na temat czasu i ilosc poprawnych odp dla kazdego z 4 poziomow
struct czas_licznik
{
	double czasOdp;
	int licznikOdp;
};

//liczba rownan na poziom i ilosc poziomow
const int ileRownan = 5;
const int ileLVL = 4;

//Glowne funkcje
int mainMenu();
int test();

//Funkcje generujace zawartosc poziomow
int genLVL1(poziom*, string&, int);
int genLVL2(poziom*, string&, int);
int genLVL3(poziom*, string&, int);
int genLVL4(poziom*, string&, int);
int wyborGen(poziom*, string&, int, int);

//Funckje zbiorcze (wywoluja inne funkcje i je zapetlaja)
int interfejs(poziom*, int, int, string, int, int, double, czas_licznik*);
int LVL(poziom*, int, int&, double&, czas_licznik*, int);

//Funkcje losujace
int randomizer(bool);
void generujLiczby(poziom*, int);
char genZnak(int);

//Funkcje wypisujace zawartosc na ekranie
int tekstMenu(int);
int instrukcja();
void interfejsGora(poziom*, int);
void wyniki(poziom*, int, double, czas_licznik*);
string trudnosc(int);

//Funkcje odpowiadajace za mechanizm zamiany na ONP i liczenia wynikow
string zamienNaONP(string);
bool czyOp(char);
int priorytet(char);
double obliczONP(string);
double oblicz(double, double, char);

int main()
{
	srand(time(NULL));
	mainMenu();
	
	return 0;
}

//Funkcja odpowiada za dzialanie menu
int mainMenu()
{
	char opcja;
	
	tekstMenu(1);
	cin >> opcja;	
	switch(opcja)
		{
   	    case '1': test();
              break;
        case '2': instrukcja();
              break;
        case '3': exit(1);
        	  break;
    	default:
    		{
    			system("cls");
    			mainMenu();
			}
		}
	return 0;
}

//funkcja wypisuje menu na ekranie
int tekstMenu(int opcja)
{
	switch(opcja)
	{
		case 1: {
				cout << endl;
				cout << setw(18) << "Glowne MENU" << endl;
				cout << "--------------------------" << endl;
				cout << " 1. Zacznij test" << endl;
				cout << " 2. Instrukcja" << endl;
				cout << " 3. Koniec programu" << endl;
				break;
				}
	}
	
	return 0;
}

int instrukcja()
{
	char wybor;
	system("cls");
	
	cout << " ------------------------------------ " << endl;
	cout << " Zanim przystapisz do testu zapoznaj sie z ta instrukcja" << endl << endl;
	cout << " Test sprawdzi Twoje zdolnosci analityczne, gdy rozpoczniesz" << endl;
	cout << " na ekranie zobaczysz rownanie matematyczne. Twoim zadaniem" << endl;
	cout << " jest jak najszybciej podac jego wynik. Nalezy zauwazyc, ze" << endl;
	cout << " posiadasz nieograniczony czas na odpowiedz, jednak im " << endl;
	cout << " szybsza odpowiedz tym lepszy Twoj sredni czas. Sam test " << endl;
	cout << " sklada sie z 4 poziomow trudnosci, w kazdym Masz do rozwiazania " << endl;
	cout << " po 5 rownan. Poziom trudnosci sie zwieksza z kazdym nowym " <<  endl;
	cout << " poziomem. W gornej czesci ekranu bedzie prezentowany aktualny " << endl;
	cout << " poziom trudnosci i ilosc poprawnie rozwiazanych rownan." << endl; 
	cout << " Test konczy sie pokazaniem wynikow w przypadku rozwiazania wszystkich" << endl;
	cout << " rownan lub zlej odpowiedzi w trakcie. " << endl << endl;
	cout << " Wazne informacje" << endl;
	cout << " Rownania pokazywane w trakcie testu sa calkowicie losowe " << endl;
	cout << " (moga sie trafic bardzo latwe, lub bardzo trudne), wiec aby" << endl;
	cout << " osiagnac zadowalajacy wynik powinno sie rozwiazac test " << endl;
	cout << " kilkukrotnie, porownac wyniki i wyciagnac z nich srednia." << endl << endl;
	cout << " Wyniki dzielenia nalezy zapisac maksymalnie do 1 miejsca po " << endl;
	cout << " przecinku przy czym nalezy je zaokraglic." << endl;
	cout << " Przykladowe zaokraglenia: "<< endl;
	cout << " 17.75 -> 17.8, 16.333 -> 16.3, 19.94 -> 19.9, 20.666 -> 20.7" << endl << endl;
	cout << " Teraz jestes gotowy(a) do podjecia proby, POWODZENIA " << endl << endl << endl;
	cout << " Kliknij 1 aby powrocic do MENU ";
	 
	cin >> wybor;
	
	switch(wybor)
	{
		case '1':
			{
				system("cls");
				mainMenu();
				break;
			}
		default:
			{
				instrukcja();
			}
	}
		
	return 0;
}


//funkcja uruchamia test
int test()
{
	int licznikGlobalny = 0;
	double czasGlobalny = 0;
	
	poziom *lvl;
	lvl = new poziom[ileRownan];

	czas_licznik *daneLokalne;
	daneLokalne = new czas_licznik[ileLVL];
	
	system("cls");
	
	for (int i = 0; i < ileLVL; i++) //zerowanie licznikow
	{
		daneLokalne[i].licznikOdp = 0;
		daneLokalne[i].czasOdp = 0;
	}
	
	for (int i = 0; i < ileLVL; i++) //wywolywanie kolejnych poziomow
	{
		LVL(lvl, ileRownan, licznikGlobalny, czasGlobalny, daneLokalne, i);
	}
	
	wyniki(lvl, licznikGlobalny, czasGlobalny, daneLokalne);
	
	delete[] lvl;
	delete[] daneLokalne;
	
	return 0;
}

//Funkcja sklada kolejne poziomy w calosc, aktualizuje liczniki czasu i dobrych odp
int LVL(poziom lvl[], int ileRownan, int& licznikGlobalny, double& czasGlobalny, czas_licznik dane[], int i)
{
	dane[i].licznikOdp = 0;

	for(int j = 0; j < ileRownan; j++)
	{
		string rownanie = "";
		wyborGen(lvl, rownanie, j, i);
		interfejs(lvl, ileRownan, i, rownanie, j, licznikGlobalny, czasGlobalny, dane);
		
		licznikGlobalny++;
		czasGlobalny += lvl[j].czas;
		
		dane[i].czasOdp += lvl[j].czas;
		dane[i].licznikOdp++;
	}
	
	return 0;
}


//Wybor sposobu generowania poziomu
int wyborGen(poziom lvl[], string& rownanie, int i, int wybor)
{
	switch (wybor)
	{
		case 0: genLVL1(lvl, rownanie, i);
			break;
		case 1: genLVL2(lvl, rownanie, i);
			break;
		case 2: genLVL3(lvl, rownanie, i);
			break;
		case 3: genLVL4(lvl, rownanie, i);
	}
}

//Generowanie 5 etapow poziomu 1
int genLVL1(poziom lvl1[], string& rownanie, int i)
{
	lvl1[i].op1 = genZnak(2); //ogolna zasada generowania liczb i znakow polega na wygenerowaniu znaku, a potem dobraniu do niego odpowiednich liczb z danych przedzialow
	lvl1[i].a = randomizer(0);
	lvl1[i].b = randomizer(0);
		
	ostringstream temp;
	temp << lvl1[i].a << lvl1[i].op1 << lvl1[i].b;
	string s = temp.str();
	
	rownanie = s;
	
	string ONP = zamienNaONP(s);
	lvl1[i].popOdp = obliczONP(ONP);

	return 0;
}


//Generowanie 5 etapow poziomu 2
int genLVL2(poziom lvl2[], string& rownanie, int i)
{
	lvl2[i].op1 = genZnak(4);
	if(lvl2[i].op1 == '*')
	{
		bool losuj = rand()%2;
		if (losuj == 0)
		{
			lvl2[i].a = randomizer(1);
			lvl2[i].b = randomizer(0);
		}
		else 
		{
			lvl2[i].a = randomizer(0);
			lvl2[i].b = randomizer(1);	
		}	
		}
	else if(lvl2[i].op1 == '/')
	{
		lvl2[i].a = randomizer(0);
		lvl2[i].b = randomizer(1);
	}	
	else 
	{
		lvl2[i].a = randomizer(0);
		lvl2[i].b = randomizer(0);
	}		

	ostringstream temp;
	temp << lvl2[i].a << lvl2[i].op1 << lvl2[i].b;
	string s = temp.str();
	
	rownanie = s;
			
	string ONP = zamienNaONP(s);
	lvl2[i].popOdp = obliczONP(ONP);
	lvl2[i].popOdp = round(lvl2[i].popOdp * 10.)/10.;
		
	return 0;
}


//Generowanie 5 etapow poziomu 3
int genLVL3(poziom lvl3[], string& rownanie, int i)
{
	generujLiczby(lvl3, i);
		
	ostringstream temp;
	temp << lvl3[i].a << lvl3[i].op1 << lvl3[i].b << lvl3[i].op2 << lvl3[i].c;
	string s = temp.str();
		
	rownanie = s;
	
	string ONP = zamienNaONP(s);
	lvl3[i].popOdp = obliczONP(ONP);
	lvl3[i].popOdp = round(lvl3[i].popOdp * 10.)/10.;

	return 0;
}


//Generowanie 5 etapow poziomu 4
int genLVL4(poziom lvl4[], string& rownanie, int i)
{
	ostringstream temp;
	generujLiczby(lvl4, i);
		
	while(
		(lvl4[i].op1 == lvl4[i].op2) || 
		(lvl4[i].op1 == '+' && lvl4[i].op2 == '-') || 
		(lvl4[i].op1 == '/' && lvl4[i].op2 == '*') ||   //Petla wyklucza pojawienie sie kombinacji znakow, w ktorych wstawienie nawiasu
		(lvl4[i].op1 == '*' && lvl4[i].op2 == '/') ||   // nie zmienia niczego w rownaniu
		(lvl4[i].op1 == '-' && lvl4[i].op2 == '-')
		)
	{
		generujLiczby(lvl4, i);	
	}
		
	if(lvl4[i].op1 == '-' && (lvl4[i].op2 == '-' || lvl4[i].op2 == '+'))
		{
			temp << lvl4[i].a << lvl4[i].op1 << '(' << lvl4[i].b << lvl4[i].op2 << lvl4[i].c << ')';
		}
		
	else if((lvl4[i].op1 == '-' || lvl4[i].op1 == '+') && (lvl4[i].op2 == '*' || lvl4[i].op2 == '/'))
		{
			temp << '(' << lvl4[i].a << lvl4[i].op1 << lvl4[i].b << ')' << lvl4[i].op2 << lvl4[i].c;		
		}
		
	else if((lvl4[i].op1 == '*' || lvl4[i].op1 == '/') && (lvl4[i].op2 == '+' || lvl4[i].op2 == '-'))
		{
			lvl4[i].c = randomizer(1); //jesli pierwszy znak jest mnozeniem/dzieleniem to rownanie w nawiasie zostaje sprowadzone do liczb jednocyfrowych dla ulatwienia
			if (lvl4[i].op2 == '-' && lvl4[i].b == lvl4[i].c)
			{
				lvl4[i].c = randomizer(1); // nie moze byc w nawiasie zera wynikajacego z odejmowania
				lvl4[i].b = randomizer(1);
			}
			temp << lvl4[i].a << lvl4[i].op1 << '(' << lvl4[i].b << lvl4[i].op2 << lvl4[i].c << ')';			
		}
			
	string s = temp.str();
		
	rownanie = s;
		
	string ONP = zamienNaONP(s);
	lvl4[i].popOdp = obliczONP(ONP);
	lvl4[i].popOdp = round(lvl4[i].popOdp * 10.)/10.;
	
	return 0;
}


//Funkcja zbiorcza pokazuje caly interfejs, liczy czas odp, sprawdza prawdziwosc wyniku, pokazuje tabele wynikowa w razie pomylki
int interfejs(poziom lvl[], int ileRownan, int wybor, string rownanie, int i, int licznikGlobalny, double czasGlobalny, czas_licznik dane[])
{
	clock_t start, stop;
	start = clock();
	
	lvl[i].trudnosc = trudnosc(wybor);
	
	interfejsGora(lvl, licznikGlobalny);
	
	cout << endl << setw(sizeof(lvl)+10);
	cout << rownanie << " = ";
	
	cin >> lvl[i].wynik;
	
	if(lvl[i].wynik != lvl[i].popOdp)
	{
		cout << endl;
		cout << setw(22) << "Zly wynik" << endl; 
		
		wyniki(lvl, licznikGlobalny, czasGlobalny, dane);
	}
	
	stop = clock();
	
	lvl[i].czas = (double)(stop - start) / CLOCKS_PER_SEC;

	system("cls");

	return 0;
}


//Generowanie znaku w rownianiach
char genZnak(int ileZnakow)
{
	char znak[6] = { '+', '-', '*', '/'};
	
	return znak[rand()%ileZnakow];
}


//Wypisanie gornej czesci podczas testu
void interfejsGora(poziom lvl[], int licznik)
{
	cout << " ------------------------------------ " << endl;
	cout << " Poziom: ";
	cout << setw(sizeof(lvl->trudnosc)) << lvl->trudnosc;
	cout << "  |  ";
	cout << "Dobrych odp: ";
	cout << setw(sizeof(licznik)-3) << licznik << endl;
	cout << " ------------------------------------ " << endl;
}


//Funckja przedstawiajaca wyniki testu
void wyniki(poziom lvl[], int licznikGlobalny, double czasGlobalny, czas_licznik dane[])
{
	char wybor;
	
	cout << " ------------------------------------ " << endl;
	cout << " Dobrych wynikow";
	cout << "   |  ";
	cout << "Sredni czas" << endl;
	
	for (int i = 0; i < ileLVL; i++)
	{
		if (dane[i].licznikOdp == 0)
		{
			dane[i].czasOdp = 0;
		}
		cout << " ------------------------------------ " << endl;
		cout << " LVL: ";
		cout << setw(sizeof(lvl->trudnosc)) << trudnosc(i);
		cout << setw(sizeof(int)-1) << dane[i].licznikOdp;
		cout << "  |  ";
		cout << setw(sizeof(int)) << setprecision(4) << dane[i].czasOdp/dane[i].licznikOdp << " s" << endl;	
	}
	
	cout << " ------------------------------------ " << endl;
	cout << " Ogolny wynik: ";
	cout << setw(sizeof(int)-2) << licznikGlobalny;
	cout << "  |  ";
	cout << setw(sizeof(int)) << setprecision(4) << czasGlobalny/licznikGlobalny << " s" << endl;	
	cout << " ------------------------------------ " << endl << endl;
	cout << " Dziekuje za wziecie udzialu, kliknij 1 by wrocic do MENU ";
	
	cin >> wybor;
	
	switch(wybor)
	{
		case '1': 
		{
			system("cls");
			mainMenu();
			break;
		}
		default:
			{
				system("cls");
				wyniki(lvl, licznikGlobalny, czasGlobalny, dane);
			}
		
	}
	
	system("PAUSE");
}


//Funckja generuje liczby z przedzialu 1-99 lub 1-9
int randomizer(bool n)
{
	int liczba;
	
	if(n == 0)
	{
		liczba = rand()%99 + 1;
	}
	else liczba = rand()%9 + 1;
	
	return liczba;
}

//Funkcja zwraca aktualny poziom trudnosci
string trudnosc(int wybor)
{
	switch(wybor)
	{
		case 0: return "Pierwszy";
		case 1: return "Drugi";
		case 2: return "Trzeci";
		case 3: return "Czwarty";
		default: return "Nieznany poziom";
	}
}


//Funkcja generuje liczby do poziomow 3 i 4
void generujLiczby(poziom lvl[], int i)
{
	lvl[i].a = randomizer(0);
	lvl[i].op1 = genZnak(4);
	
	if (lvl[i].op1 == '*' || lvl[i].op1 == '/')
	{
		lvl[i].b = randomizer(1);
	}
	
	else lvl[i].b = randomizer(0);
	
	lvl[i].op2 = genZnak(4);
	
	if (lvl[i].op2 == '*' || lvl[i].op2 == '/')
	{
		lvl[i].c = randomizer(1);
	}
	
	else lvl[i].c = randomizer(0);
}


//Zamiana stringa z rownaniem klasycznym na rownanie w ONP	
string zamienNaONP(string rownanie)
{
    string ONP;
    stack<char> stos;

    for(int i = 0; i < rownanie.length(); ++i)
    {
        if(isdigit(rownanie[i]))
        {
            ONP += rownanie[i];  //sprawdzenie czy kolejny znak jest cyfra i zapisanie go w stringu
        }
        else if(rownanie[i] == ' ')
        {
            continue;   //spacje pomijamy
        }
        else if(czyOp(rownanie[i]))  //sprawdzenie czy znak jest operatorem
        {
            ONP += " "; 
            
            while((!stos.empty()) &&
                (priorytet(stos.top()) >= priorytet(rownanie[i]))) // sprawdzamy priorytet operatora
            {
                ONP += stos.top(); // operator z wyzszym priorytetem na stosie laduje w stringu
                stos.pop();
            }
            
            stos.push(rownanie[i]); // operator laduje na stosie
        }
        else if(rownanie[i] == '(')
        {
            stos.push(rownanie[i]);  //nawias otwierajacy na stos
        }
        else if(rownanie[i] == ')')
        {
            while((!stos.empty()) && (stos.top() != '('))
            {
                ONP += stos.top(); //jesli napotka nawias zamykajacy wrzuca do stringa wszystko az do nawiasu otwierajacego
                stos.pop();
            }

            if(!stos.empty())
            {
                stos.pop();
            }
        }
    }
    while(!stos.empty())
    {
        ONP += stos.top(); // wrzuca do stringa wszystko az stos zostanie pusty
        stos.pop();
    }

    return ONP; 
}


//Sprawdzenie czy konkretny znak stringa to operator
bool czyOp(char op)
{
    switch(op)
    {
        case '+': ;
		case '-': ;
		case '*': ;
		case '/': ;
            return true;
            break;
        default:
           return false;
           break;
    }
}


//Wyznaczenie waznosci operatorow matematycznych
int priorytet(char op)
{
    int priorytet = 0;
    
    switch(op)
    {
        case '*': case '/':
           priorytet = 3;
           break;
        case '-':
           priorytet = 2;
           break;
        case '+':
           priorytet = 1;
           break;
    }
    
    return priorytet;
}


//Obliczenie wyniku rownania ONP
double obliczONP(string ONP)
{
	double odp = 0;
	double wyr1 = 0; // zmienne beda przechowywaly liczby ze stosu, na ktorych wykonujemy dzialanie
	double wyr2 = 0;
	stack<double> stos;
	
	for (int i = 0; i < ONP.length(); ++i)
	{
		if(isspace(ONP[i])) continue; //spacje pomijamy
		
		else if(isdigit(ONP[i]))
		{
			if(isdigit(ONP[i+1]) && ONP[i] != ' ' && (!czyOp(ONP[i])))
			{
				string temp = ""; // jesli napotka liczbe program sprawdza czy nastepny znak tez jest liczba
				temp += ONP[i];   // jesli jest to laczy je w jednego stringa tak aby na stosie mozna bylo umiescic liczby dwucyfrowe
				temp += ONP[i+1];
				stos.push(atof(temp.c_str()));
				i = i+1;
			}
			else
			{
				string temp2 = "";
				temp2 += ONP[i]; 
				stos.push(atof(temp2.c_str()));
			}
		}
		else if(czyOp(ONP[i]) && (!stos.empty())) // sprwadza czy znak jest operatorem
		{
			char znak = ONP[i];
			
			if(stos.size() > 1)
			{
				wyr2 = stos.top(); // jesli tak to do dwoch zmienny laduja kolejne liczby ze stosu
				stos.pop();
				wyr1 = stos.top();
				stos.pop();
				odp = oblicz(wyr1, wyr2, znak); // do odpowiedzi zapisuje wynik funkcji oblicz wykonanej na tych liczbach
				stos.push(odp);
			}
		}
	}
	if(!stos.empty())
	{
		odp = stos.top(); //odpowiedzia zostaje liczba na szczycie stosu
	}
	
	return odp;
}


//Funkcja wykonuje obliczenia dla dwoch argumentow z rownania ONP
double oblicz(double wyr1, double wyr2, char znak)
{
	double wynik = 0;

	switch(znak)
	{
		case '+': wynik = wyr1 + wyr2;
			break;
		case '-': wynik = wyr1 - wyr2;
			break;
		case '*': wynik = wyr1 * wyr2;
			break;
		case '/': wynik = wyr1 / wyr2;
			break;
	}
	
	return wynik;
}

