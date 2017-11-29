/*
Kolejnoœæ przegl¹dania pól to góra, dó³, lewa, prawa.
*/
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>


using namespace std;

const int rozmiar = 20;
const string nazwaPliku = "grid.txt";
struct Kratka {
	int x;
	int y;
	bool przeszkoda;
	Kratka* rodzic;
};
Kratka grid[rozmiar][rozmiar];
vector<Kratka*> otwarta;
vector<Kratka*> zamknieta;
vector<Kratka*> trasa;
bool znalezionoTrase = false;

//deklaracje funkcji
void WczytajGridZPliku(string);
void DodajSasiadowDoOtwartej(Kratka&);
void DodajDoOtwartej(int, int, Kratka&);
void DodajDoZamknietej();
void WyswietlGrid();
//void WypiszVectorKratek(const vector<Kratka*>&);
bool VectorZawieraKratke(const vector<Kratka*>&, const Kratka&);
float f(Kratka&, Kratka&);
void WypelnijVectorTrasy();
float doKwadratu(float);

int main()
{
	WczytajGridZPliku(nazwaPliku);
	Kratka start = { 0, 0, false, NULL }; // nie przeszkoda, bez rodzica
	zamknieta.push_back(&start);
	DodajSasiadowDoOtwartej(start);
	while (otwarta.size() > 0 && !znalezionoTrase) {
		DodajDoZamknietej();
		znalezionoTrase = VectorZawieraKratke(zamknieta, grid[19][19]);
		DodajSasiadowDoOtwartej(*(zamknieta.back())); //dodaje s¹siadów ostatniej kratki w liœcie zamkniêtej
	}
	if (znalezionoTrase)
		WypelnijVectorTrasy();
	else
		cout << "Trasa nie istnieje\n";
	WyswietlGrid();
	cin.ignore();
}

void DodajSasiadowDoOtwartej(Kratka& kratka)
{
	int x = kratka.x;
	int y = kratka.y;
	DodajDoOtwartej(x, y + 1, kratka);
	DodajDoOtwartej(x, y - 1, kratka);
	DodajDoOtwartej(x - 1, y, kratka);
	DodajDoOtwartej(x + 1, y, kratka);
}
void DodajDoOtwartej(int x, int y, Kratka& dodajacy)
{
	Kratka& kratka = grid[x][y];
	if (x < rozmiar && x >= 0 && y < rozmiar && y >= 0 && !grid[x][y].przeszkoda)
	{ //jesli miesci sie w gridzie i nie jest przeszkod¹ dodaj do otwartej
		if (!VectorZawieraKratke(otwarta, kratka) && !VectorZawieraKratke(zamknieta, kratka))
			otwarta.push_back(&kratka);
		float nowyF = f(kratka, dodajacy);
		if (!kratka.rodzic || nowyF < f(kratka, *kratka.rodzic)) //jesli nie ma ustawionego rodzica lub nowy F jest mniejszy podmien rodzica
			grid[x][y].rodzic = &dodajacy;
	}
}
float f(Kratka& kratka, Kratka& rodzic)
{
	float g = 0;
	Kratka* obecnieSprawdzana = &rodzic;
	while (obecnieSprawdzana)
	{
		g++; //koszt pojedynczego ruchu to zawsze 1
		obecnieSprawdzana = obecnieSprawdzana->rodzic; //po rodzicach do startu dodaj¹c po 1
	}
	float h = sqrt(doKwadratu(kratka.x - 19.f) + doKwadratu(kratka.y - 19.f));
	return g + h;
}
void DodajDoZamknietej() //wybrac kratke o najmniejszym f z otwartej, pamietac o konfliktach (hierarchicznie, pewnie po indeksie)
{
	float fMin = f(*otwarta[0], *(otwarta[0]->rodzic));
	int indeksFMin = 0;
	for (int i = 0; i < otwarta.size(); i++) //petla znajduj¹ca minimaln¹ wartoœæ f
											 //konflikty rozwi¹zuj¹ siê same, przy jednakowych minimalnych wartoœciach f wybrana zostanie ta o mniejszym indeksie 
											 //w wektorze "otwarta", a wiêc ta dodana wczeœniej (hierarchicznie)
	{
		float wartoscF = f(*otwarta[i], *otwarta[i]->rodzic);
		if (wartoscF < fMin)
		{
			fMin = wartoscF;
			indeksFMin = i;
		}
	}
	zamknieta.push_back(otwarta[indeksFMin]); //dodaje kratke o najmniejszym f z otwartej do zamknietej
	otwarta.erase(otwarta.begin() + indeksFMin); //i usuwa j¹ z otwartej
}
void WypelnijVectorTrasy()
{
	Kratka* obecna = zamknieta.back(); //ostatnia kratka w zamknietej to cel
	do
	{
		trasa.push_back(obecna);
		obecna = obecna->rodzic;
	} while (obecna);
}
void WczytajGridZPliku(string nazwaPliku)
{
	ifstream plik(nazwaPliku.c_str()); //wywoluje konstruktor ifstream
	if (plik.good()) {

		for (int y = 0; y < rozmiar; y++)
		{
			for (int x = 0; x < rozmiar; x++)
			{
				int pom;
				bool przeszkoda;
				plik >> pom;
				if (pom == 0)
					przeszkoda = false;
				else if (pom == 5)
					przeszkoda = true;
				else continue;
				//cout << "creating " << i << " " << j << "\n";
				//cout << pom << "\n";
				grid[x][rozmiar - 1 - y] = Kratka{ x, rozmiar - 1 - y, przeszkoda, NULL }; //odwrocenie bo y=0 na dole
			}
			//cout << "\n";
		}
	}
	plik.close();
}
void WyswietlGrid()
{
	for (int y = 19; y >= 0; y--)
	{
		for (int x = 0; x < 20; x++)
		{
			//cout << (grid[x][y].x) << "," << grid[x][y].y << " "; //wyswietlenie wspolrzednych
			char oznaczenie = (grid[x][y].przeszkoda ? '5' : '0');
			if (VectorZawieraKratke(trasa, grid[x][y]))
				oznaczenie = '3';
			cout << oznaczenie << " ";
		}
		cout << "\n";

	}
}
//void WypiszVectorKratek(const vector<Kratka*>& wektor)
//{
//	for (int i = 0; i < wektor.size(); i++)
//		cout << wektor[i]->x << "," << wektor[i]->y << "\n";
//}
bool VectorZawieraKratke(const vector<Kratka*>& wektor, const Kratka& kratka)
{
	for (int i = 0; i < wektor.size(); i++)
		if (wektor[i]->x == kratka.x && wektor[i]->y == kratka.y)
			return true;
	return false;
}
float doKwadratu(float a) {
	return a*a;
}

