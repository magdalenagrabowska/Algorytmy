#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <cstring>
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

void zamiana(element& tab, element& tab2)
{
    swap(tab.r, tab2.r);
    swap(tab.p, tab2.p);
    swap(tab.q, tab2.q);
    swap(tab.nr, tab2.nr);
}

void heapify_max(element tab[], int liczba_zadan, int i) // najwieksze q
{
    int largest = i;  // Inicjalizacja najwiekszego jako korzen
    int l = 2 * i +1; //indeks lewego dziecka
    int p = 2 * i + 2; //indeks prawego dziecka

    if (l <liczba_zadan && tab[l].q > tab[largest].q)
    {
        largest = l;
    }

    if (p < liczba_zadan && tab[p].q > tab[largest].q)
    { 
        largest = p;
    }

    if (largest != i)
    {
        zamiana(tab[i], tab[largest]);

        
        heapify_max(tab, liczba_zadan, largest); //rekursywne budowanie kopca
    }
}

void heapify_min(element tab[], int liczba_zadan, int i) // najmniejsze r
{
    int min = i; // Inicjalizacja najmniejszego jako korzen
    int l = 2 * i +1; //indeks lewego dziecka
    int p = 2 * i + 2; //indeks prawego dziecka

    if (liczba_zadan > l && tab[min].r > tab[l].r)
    {
        min = l;
    }

    if (liczba_zadan > p && tab[min].r > tab[p].r)
    {
        min = p;
    }

    if (min != i)
    {
        zamiana(tab[i], tab[min]);
        heapify_min(tab, liczba_zadan, min); //rekursywne budowanie kopca
    }
}

void buildHeapmax(element tab[], int liczba_zadan) //budowanie kopca z najwiekszym korzeniem
{
    int startIdx = (liczba_zadan/2)-1;
    for (int i = startIdx; i >= 0; i--)
    {
        heapify_max(tab, liczba_zadan, i);
    }
}

void buildHeapmin(element tab[], int liczba_zadan) //budowanie kopca z najmniejszym korzeniem
{
    int startIdx = (liczba_zadan/2)-1;
    for (int i = startIdx; i >= 0; i--)
    {
        heapify_min(tab, liczba_zadan, i);
    }
}

void deleteRoot_max(element tab[], int& liczba_zadan)
{
    //zamiana ostatniego elementu z korzeniem 
    tab[0].nr = tab[liczba_zadan - 1].nr;
    tab[0].r = tab[liczba_zadan - 1].r;
    tab[0].p = tab[liczba_zadan - 1].p;
    tab[0].q = tab[liczba_zadan - 1].q;
    liczba_zadan = liczba_zadan - 1; //zmniejszenie wielkosci kopca minimalnego
    heapify_max(tab, liczba_zadan, 0);
}

void deleteRoot_min(element tab[], int& liczba_zadan)
{
    tab[0].nr = tab[liczba_zadan - 1].nr;
    tab[0].r = tab[liczba_zadan - 1].r;
    tab[0].p = tab[liczba_zadan - 1].p;
    tab[0].q = tab[liczba_zadan - 1].q;
    liczba_zadan = liczba_zadan - 1; 
    heapify_min(tab, liczba_zadan, 0);
}

void insertNode_max(element tab[], int& liczba_zadan, element e_dod)// e_dod to element, który dodajemy
{

    liczba_zadan = liczba_zadan + 1; //zwiekszenie wielkosci kopca maksymalnego
    tab[liczba_zadan - 1].nr = e_dod.nr;
    tab[liczba_zadan - 1].r = e_dod.r;
    tab[liczba_zadan - 1].p = e_dod.p;
    tab[liczba_zadan - 1].q = e_dod.q;
    heapify_max(tab, liczba_zadan, liczba_zadan - 1);
}

void insertNode_min(element tab[], int& liczba_zadan, element e_dod)
{
    liczba_zadan = liczba_zadan + 1; //zwiekszenie wielkosci kopca minimalnego
    tab[liczba_zadan - 1].nr = e_dod.nr;
    tab[liczba_zadan - 1].r = e_dod.r;
    tab[liczba_zadan - 1].p = e_dod.p;
    tab[liczba_zadan - 1].q = e_dod.q;
    heapify_min(tab, liczba_zadan, liczba_zadan - 1);
}

void oblicz_cmax(element tab[], element N[], element G[], int liczba_zadan)
{
    element elem;
    int k = 0;
    int t = 0;
    int Cmax = 0;
    int* pi = new int[liczba_zadan];
    int rozmiar_heap_G = 0;
    int rozmiar_heap_N = liczba_zadan;

    while (rozmiar_heap_G != 0 || rozmiar_heap_N != 0)  //Gdy G==NULL(wszystkie zadania wykonane) i N==NULL(wszystkie zadania przygotowane) zakonczenie pracy
    {

        while (rozmiar_heap_N != 0 && N[0].r <= t)
        {
            elem = N[0];
            insertNode_max(G, rozmiar_heap_G, elem); //dodanie do zbioru G zadania gotowego do wykonania alternatywa funkci push 
            deleteRoot_min(N, rozmiar_heap_N); //usuniecie ze zbioru N zadania gotowego do wykonania alternatywa funkci pop
        }

        if (rozmiar_heap_G == 0)
            t = N[0].r;
        else
        {
            buildHeapmax(G, rozmiar_heap_G); //ponowne zbudowanie kopca G
            elem = G[0];
            deleteRoot_max(G, rozmiar_heap_G);
            pi[k] = elem.nr;
            k += 1;
            t = t + elem.p;
            Cmax = max(Cmax, t + elem.q);
        }
    }
    cout << "Czas Cmax algorytmu Schrage bez podzialu: " << Cmax << endl;

    cout << "Permutacja: ";
    for (int i = 0; i < liczba_zadan; i++)
    {
        cout << " " << pi[i] + 1;
    }
    cout << endl;
}

void oblicz_cmax_podzial(element tab[], element N[], element G[], int liczba_zadan)
{

    element l, e;
    l = { 0 };
    int Cmax = 0;
    int t = 0;
    int rozmiar_heap_G = 0;
    int rozmiar_heap_N = liczba_zadan;
    while (rozmiar_heap_G != 0 || rozmiar_heap_N != 0)
    {
        while (rozmiar_heap_N != 0 && N[0].r <= t)
        {
            e = N[0];
            insertNode_max(G, rozmiar_heap_G, e);
            deleteRoot_min(N, rozmiar_heap_N);
            if (e.q > l.q)
            {
                l.p = t - e.r;
                t = e.r;
                if (l.p > 0)
                {
                    insertNode_max(G, rozmiar_heap_G, l);
                }
            }
        }

        if (rozmiar_heap_G == 0)
            t = N[0].r;
        else
        {
            buildHeapmax(G, rozmiar_heap_G);
            e = G[0];
            deleteRoot_max(G, rozmiar_heap_G);
            l = e;
            t = t + e.p;
            Cmax = max(Cmax, t + e.q);
        }
    }
    cout << "Czas Cmax algorytmu Schrage z podzialem: " << Cmax << endl;
}

void reset(element tab[], element N[], element G[], int liczba_zadan){
    for (int i = 0; i < liczba_zadan; i++)
    {
        N[i].r = tab[i].r;
        N[i].p = tab[i].p;
        N[i].q = tab[i].q;
        N[i].nr = i;
        G[i].r = 0;
        G[i].p = 0;
        G[i].q = 0;
        G[i].nr = i;
    }
    buildHeapmin(N, liczba_zadan);


}

void wyswietlanie(element tab[],int liczba_zadan) // wyswietlanie wynikow, potrzebne do sprawdzenia kolejnosci
{
    for (int i = 0; i < liczba_zadan; i++)
    {
        // cout<<tab[i].nr+1<<" ";
        cout << endl;
        cout << tab[i].r << " " << tab[i].p << " " << tab[i].q;
    }
    cout << endl;
}

int main()
{
    element tab[100];
    element N[100];
    element G[100];
    int liczba_zadan;
    string exp[9] = { "data.000:", "data.001:", "data.002:", "data.003:", "data.004:", "data.005:", "data.006:", "data.007:", "data.008:" };
    string tmp;
    int t = 0;
    int time_podzial = 0;
    int time = 0;

    cout << "SCHRAGE NA KOPCU NA TABLICACH" << endl << endl;

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
        cout <<  exp[p] << endl;
        reset(tab, N, G, liczba_zadan); //implementacja kopców N i G

        auto start_podzial = chrono::steady_clock::now();
        oblicz_cmax_podzial(tab,N ,G,liczba_zadan);
        auto stop_podzial = chrono::steady_clock::now();

        cout << "Czas sortowania:" << chrono::duration_cast<chrono::microseconds>(stop_podzial - start_podzial).count() << " us" << endl;

        reset(tab, N, G, liczba_zadan); //reset wartosci

        auto start = chrono::steady_clock::now();
        oblicz_cmax(tab, N, G, liczba_zadan);
        auto stop = chrono::steady_clock::now();

        cout << "Czas sortowania:" << chrono::duration_cast<chrono::microseconds>(stop - start).count() << " us" << endl;
        cout << "----------------------------------------------------------" << endl;
        plik.close();

        time += chrono::duration_cast<chrono::microseconds>(stop - start).count();
        time_podzial += +chrono::duration_cast<chrono::microseconds>(stop_podzial - start_podzial).count();
    }

    cout << "Calkowity czas dla wszystkich danych:" << endl;
    cout << "Shrage z podzialem: " << time_podzial << " us" << endl;
    cout << "Shrage bez podzialu: " << time << " us" << endl;
    return 0;
}
