#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <chrono>



using namespace std;

struct element
{
public:
    int r;  // czas dostarczania elementu
    int p;  // czas obsługiwania elementu przez maszynę
    int q;  // czas stygnięcia elementu
    int nr; // numer zadania
};

bool sort_R(const element& tab, const element& tab2) 
{
    return tab.r > tab2.r;
}

bool sort_Q(const element& tab, const element& tab2)
{
    return tab.q < tab2.q;
}



void oblicz_cmax(vector<element> N, vector<element> G, int liczba_zadan)
{

    element e;
    int k = 0;
    int Cmax = 0;
    int* pi = new int[liczba_zadan];
    int t = 0;
    while (G.empty()==false || N.empty()==false)
    {

        while (N.empty()==false && N.back().r <= t)
        {

            e = N.back();
            G.push_back(e);
            //cout << "Dodano :" << e.nr + 1 << "  " << e.r << "  " << e.p << "  " << e.q << endl;
            N.pop_back();

        }

        if (G.empty()==true)
        {
            t = N.back().r;
        }
        else
        {
 
            sort(G.begin(), G.end(), sort_Q); //sortowanie Zbioru zadan do wykonania wedlug Q
            e = G.back();
            //cout << "usunieto :" << e.nr + 1 << "  " << e.r << "  " << e.p << "  " << e.q << endl;
            G.pop_back();
            pi[k] = e.nr;
            k += 1;
            t = t + e.p;
            Cmax = max(Cmax, t + e.q);
        }


    }
    cout << "Czas Cmax algorytmu Schrage bez podzialu: " << Cmax << endl;

    cout << "Permutacja: ";
    for (int i = 0; i < liczba_zadan; i++) {

        cout << " " << pi[i] + 1;

    }
    cout << endl;
    delete[]pi;
}

void oblicz_cmax_z_podzialem(vector<element> N, vector<element> G)
{

    element e;
    element l;
    l = { 0 };
    int Cmax = 0;
    int t = 0;
    e.q = 99999999;
    while (G.empty() == false || N.empty() == false)
    {

        while (N.empty() == false && N.back().r <= t)
        {
            e = N.back();
            G.push_back(e);
            N.pop_back();

            if (e.q > l.q)
            {
                l.p = t - e.r;
                t = e.r;

                if (l.p > 0)
                {
                    G.push_back(l);
                }

            }

        }

        if (G.empty() == true)
        {
            t = N.back().r;
        }
        else
        {
            sort(G.begin(), G.end(), sort_Q); //posortowanie wedlug najwiekszego Q
            e = G.back();
            G.pop_back();
            l = e;
            t = t + e.p;
            Cmax = max(Cmax, t + e.q);
        }
    }
    cout << "Czas Cmax algorytmu Schrage z podzialem: " << Cmax << endl;
}


void wyswietlanie(int liczba_zadan, element tab[]) // wyswietlanie wynikow, potrzebne do sprawdzenia kolejnosci
{
    for (int i = 0; i < liczba_zadan; i++)
    {
        // cout<<tab[i].nr+1<<" ";
        cout << endl;
        cout << tab[i].r << tab[i].p << tab[i].q << endl;
    }
    cout << endl;
}

int main()
{
    element tab[100];
    int liczba_zadan;
    string exp[9] = { "data.000:", "data.001:", "data.002:", "data.003:", "data.004:", "data.005:", "data.006:", "data.007:", "data.008:" };
    string tmp;
    int time_podzial = 0;
    int time = 0;

    cout << "SCHRAGE NA STOSACH BIBLIOTEKA STL" << endl << endl;

    for (int p = 0; p < 9; p++)
    {
        ifstream plik("schrage_data.txt", ios::in);
        while (tmp != exp[p])
            plik >> tmp;
        plik >> liczba_zadan;

        for (int i = 0; i < liczba_zadan; i++)
        {
            plik >> tab[i].r >> tab[i].p >> tab[i].q;
            tab[i].nr = i;

        }
        vector<element> N(tab, tab + liczba_zadan); //inicjalizacja vectora elementow do stosu
        vector<element> G; //inicjalizacja vectora elementow do stosu
        plik.close();

        //deklaracja stosow z biblioteki STL algorithm
        make_heap(N.begin(), N.end(), sort_R); //minimalny stos
        make_heap(G.begin(), G.end(), sort_Q); //maksymalny stos

        sort(N.begin(), N.end(), sort_R); //posortowanie N wedlug najmniejszego R

        cout << exp[p] << endl;

        auto start_podzial = chrono::steady_clock::now();
        oblicz_cmax_z_podzialem(N, G);
        auto stop_podzial = chrono::steady_clock::now();

        cout <<"Czas sortowania:" << chrono::duration_cast<chrono::microseconds>(stop_podzial - start_podzial).count() <<" us" << endl;

        auto start = chrono::steady_clock::now();
        oblicz_cmax(N, G, liczba_zadan);
        auto stop = chrono::steady_clock::now();
        
        cout << "Czas sortowania:" << chrono::duration_cast<chrono::microseconds>(stop - start).count() << " us" << endl;
        cout << "----------------------------------------------------------" << endl;

        time += chrono::duration_cast<chrono::microseconds>(stop - start).count();
        time_podzial += +chrono::duration_cast<chrono::microseconds>(stop_podzial - start_podzial).count();
    }
    cout << "Calkowity czas dla wszystkich danych:" << endl;
    cout << "Shrage z podzialem: " << time_podzial<<" us" << endl;
    cout << "Shrage bez podzialu: " << time << " us" << endl;
    return 0;
}
