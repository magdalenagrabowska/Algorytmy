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
     int p;  // czas obs³ugiwania elementu przez maszynê
     int q;  // czas stygniêcia elementu
     int nr; // numer zadania
};


element min_R(element* tab, int liczba_zadan){ ////zwracanie pozycji tablicy dla najmniejszego R
    int minr = 999999999;
    int tmp=0;
    for (int i = 0; i < liczba_zadan; i++) {
       
        if (minr >= tab[i].r)
        {
            minr = tab[i].r;
            tmp = i;

        }
       
    }

    return tab[tmp];
}

element max_Q(element* tab, int liczba_zadan) { //zwracanie pozycji tablicy dla najwiekszego Q
    int maxq = 0;
    int tmp = 0;
    for (int i = 0; i <= liczba_zadan; i++) {

        if (maxq < tab[i].q)
        {
            maxq = tab[i].q;
            tmp = i;

        }
    }

    return tab[tmp];
}

void oblicz_cmax_podzial(element* N, element* G,  int liczba_zadan )
{
     element l;
     element e;
     l = { 0 };
     int Cmax=0;
     int tmp = 0; 
     int tmp2 = 0;
     int tmp3 = 0;
     int t = 0;
     while (tmp - tmp2 != 0 || tmp != liczba_zadan) //Gdy G==NULL(wszystkie zadania wykonane) i N==NULL(wszystkie zadania przygotowane) zakonczenie pracy
     {

         while (tmp != liczba_zadan && min_R(N, liczba_zadan - tmp).r <= t) //zakonczenie petlu gdy N==NULL lub najmniejsze czas gotowosci jest mniejszy lub równy t
         {
             e = min_R(N, liczba_zadan - tmp); //znalezienie nie gotowego do zrobienia procesu z najmniejszym R
             G[tmp - tmp2 ] = e;
             //cout << "Dodano :" << e.nr +1<< "  " << e.r << "  " << e.p << "  " << e.q << endl; 
             for (int i = 0; i < liczba_zadan - tmp; i++) { //usuniecie ze zbioru gotowego procesu przez przesuwanie w lewo i zmniejszenie liczby zadan
                 if (N[i].nr == e.nr)
                 {
                     for (int x = i; x < liczba_zadan - 1; x++) {
                         N[x].r = N[1 + x].r;
                         N[x].p = N[1 + x].p;
                         N[x].q = N[1 + x].q;
                         N[x].nr = N[1 + x].nr;
                     }
                 }

             }
             if (e.q > l.q)
             {
                 l.p = t - e.r;
                 t = e.r;
                 if (l.p > 0)
                 {
                     tmp2--; //zmniejszenie zbioru G
                     G[tmp - tmp2] = l;

                 }
             }
             tmp++; //zmniejszenie zbioru N (zwiekszenie zbioru G)
         }

         if (tmp - tmp2  == 0) //jezeli G==NULL
         {
             t = min_R(N, liczba_zadan - tmp).r;
         }
         else
         {
             e = max_Q(G, tmp - tmp2); //znalezienie gotowego do zrobienia procesu z najwiekszym Q

             //cout << "Usunieto :" << e.nr + 1 << "  " << e.r << "  " << e.p << "  " << e.q << endl;
             for (int i = 0; i < tmp - tmp2; i++) {   //usuniecie ze zbioru gotowego procesu przez przesuwanie w lewo i zwiejszenie liczby zadan
                 if (G[i].nr == e.nr)
                 {

                     for (int x = i; x < liczba_zadan - 1; x++) {
                         G[x].r = G[1 + x].r;
                         G[x].p = G[1 + x].p;
                         G[x].q = G[1 + x].q;
                         G[x].nr = G[1 + x].nr;
                     }
                 }
             }

             l = e;
             t = t + e.p;
             tmp2++; //zmnieszenie zbioru G
             Cmax = max(Cmax, t + e.q);

         }
     }
    
     cout << "Czas Cmax algorytmu Schrage z podzialem: " << Cmax << endl;
}

void oblicz_cmax(element* N, element*G,int liczba_zadan)
{

    element e;
    int k = 0;
    int Cmax = 0;
    int *pi=new int[liczba_zadan];
    int tmp = 0; 
    int tmp2 = 0;
    int t = 0;

    while (tmp - tmp2 != 0 || tmp != liczba_zadan) //Gdy G==NULL(wszystkie zadania wykonane) i N==NULL(wszystkie zadania przygotowane) zakonczenie pracy
    {

        while (tmp != liczba_zadan && min_R(N, liczba_zadan-tmp).r <= t) 
        {
            e = min_R(N, liczba_zadan-tmp);
            G[tmp-tmp2]=e;
            //cout << "Dodano :" << e.nr +1<< "  " << e.r << "  " << e.p << "  " << e.q << endl; 
            for (int i = 0; i < liczba_zadan-tmp; i++) { //usuniecie ze zbioru procesu gotowego do wykonamia przez przesuwanie w lewo i zmniejszenie liczby zadan
                if (N[i].nr == e.nr)
                {
                    for(int x=i;x<liczba_zadan-1;x++){
                    N[x].r = N[1+x].r;
                    N[x].p = N[1+x].p;
                    N[x].q = N[1+x].q;
                    N[x].nr = N[1 + x].nr;
                    }
                }

            }
            tmp++; //zmniejszenie zbioru N (zwiekszenie zbioru G)
        }

        if (tmp-tmp2==0)
        {
            t = min_R(N, liczba_zadan - tmp).r;
        }   
        else
        {
            e = max_Q(G, tmp-tmp2);

            //cout << "Usunieto :" << e.nr + 1 << "  " << e.r << "  " << e.p << "  " << e.q << endl;
            for (int i = 0; i < tmp-tmp2; i++) {   //usuniecie ze zbioru gotowego procesu przez przesuwanie w lewo
                if (G[i].nr == e.nr)
                {

                    for (int x = i; x < liczba_zadan-1; x++) {
                        G[x].r = G[1 + x].r;
                        G[x].p = G[1 + x].p;
                        G[x].q = G[1 + x].q;
                        G[x].nr = G[1 + x].nr;
                    }
                }
            }

            pi[k] = e.nr; 
            k += 1;
            t = t + e.p;
            tmp2++; //zmniejszenie zbioru G
            Cmax = max(Cmax, t + e.q);

        }
    }
    cout << "Czas Cmax algorytmu Schrage bez podzialu: " << Cmax << endl;

    cout << "Permutacja: ";
    for (int i = 0; i < liczba_zadan; i++) {

       cout<<" " << pi[i]+1;

    }
    cout << endl;
    delete[]pi;
}

void wyswietlanie(element tab[], int liczba_zadan) // wyswietlanie wynikow, potrzebne do sprawdzenia kolejnosci
{
     for (int i = 0; i < liczba_zadan; i++)
     {
          cout << endl;
          cout <<tab[i].nr+1<< " : " << tab[i].r << " " << tab[i].p << " " << tab[i].q;
     }
     cout << endl;
}

void reset(element tab[], element tabN[], element tabG[],int liczba_zadan) {  //uzyty do resetu danych w tablicy pomocniczej N i G

    for (int i = 0; i < liczba_zadan; i++)
    {

        tabN[i].r = tab[i].r;
        tabN[i].p = tab[i].p;
        tabN[i].q = tab[i].q;
        tabN[i].nr = tab[i].nr;
        tabG[i].r = 0;
        tabG[i].p = 0;
        tabG[i].q = 0;
        tabN[i].nr = tab[i].nr;
    }

}

int main()
{
     element tab[100];
     int liczba_zadan;
     string exp[9] = {"data.000:", "data.001:", "data.002:", "data.003:", "data.004:", "data.005:", "data.006:", "data.007:", "data.008:"};
     string tmp;
     int time_podzial = 0;
     int time = 0;

     cout << "SCHRAGE NA TABLICACH" << endl<< endl;

     for (int p = 0; p < 9; p++)
     {
          ifstream plik("schrage_data.txt", ios::in);
          while (tmp != exp[p])
               plik >> tmp;
          plik >> liczba_zadan;

          for (int i = 0; i < liczba_zadan; i++)
          {
               plik>> tab[i].r >> tab[i].p >> tab[i].q;
               tab[i].nr = i;
          }
          plik.close();

          //wyswietlanie(liczba_zadan,tab);
         
          //implementacja zadan na 2 tablicach (tabN- zadania nie gotowe(równe tab), G-zerowa tablica)
          element* tabN = new element[liczba_zadan]; //zadania czekajace
          element* tabG = new element[liczba_zadan]; //zadania gotowe do wykonania
    
          cout << exp[p] << endl;
          reset(tab, tabN, tabG, liczba_zadan); //wyzerowanie G i przypisanie niewykonanych zadan do tablicy N

          auto start_podzial = chrono::steady_clock::now();
          oblicz_cmax_podzial(tabN, tabG, liczba_zadan);
          auto stop_podzial = chrono::steady_clock::now();

          cout << "Czas sortowania:" << chrono::duration_cast<chrono::microseconds>(stop_podzial - start_podzial).count() << " us" << endl;

          reset(tab, tabN, tabG,liczba_zadan);

          auto start = chrono::steady_clock::now();
          oblicz_cmax(tabN, tabG, liczba_zadan);
          auto stop = chrono::steady_clock::now();

          cout << "Czas sortowania:" << chrono::duration_cast<chrono::microseconds>(stop - start).count() << " us" << endl;
          cout << "----------------------------------------------------------" << endl;

          time += chrono::duration_cast<chrono::microseconds>(stop - start).count();
          time_podzial += +chrono::duration_cast<chrono::microseconds>(stop_podzial - start_podzial).count();
     }

     cout << "Calkowity czas dla wszystkich danych:" << endl;
     cout << "Shrage z podzialem: " << time_podzial << " us" << endl;
     cout << "Shrage bez podzialu: " << time << " us" << endl;
     return 0;
}
