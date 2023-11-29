/* Projeto realizado por:
    José António Lopes LEIC-A 1ºano nº103938
*/


/* Import das várias bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* Definição de constantes */
#define MAX_FLIGHT 30000
#define MAX_AEROPORT 40
#define MAX_ID 4
#define MAX_COUNTRY 31
#define MAX_CITY 55
#define MAX_CODE_FLIGHT 7
#define IN 1
#define OUT 0

/* vetor no qual o índice + 1 corresponde a um mês do ano e o conteúdo é o número de dias desse mês */
int days_of_month[12] = {31, 28, 31 ,30 , 31, 30, 31, 31, 30, 31, 30, 31};

/* Definição da estrutura das horas*/
typedef struct time
{
    int hours;
    int minutes;
} Time;

/* Definição da estrutura da data */
typedef struct date
{
    int day;
    int month;
    int year;
} Date;

/* Definição da estrutura dos aeroportos */
typedef struct aeroport
{
    char id[MAX_ID];
    char country[MAX_COUNTRY];
    char city[MAX_CITY]; /* Tamanho maior que 51 para contar com espaços */
    int num_flights;
} Aeroport;

/* Definição da estrutura dos voos */
typedef struct flight
{
    char code[MAX_CODE_FLIGHT];
    char aeroport_out_id[MAX_ID];
    char aeroport_arrive_id[MAX_ID];
    Date date;
    Time hour;
    Time duration;
    int max_passengers;
} Flight;

/* Definição de variáveis globais */
int i_a = 0, i_id = 0, i_v = 0, i_c = 0, i_p = 0;
Aeroport aeroports[MAX_AEROPORT], temp_aeroport;
Flight temp_flight;
Flight flights[MAX_FLIGHT], flights_aux_p[MAX_FLIGHT], flights_aux_c[MAX_FLIGHT];
Date date = {1, 1, 2022}, new_date;

/* Função que copia o aeroport_1 para o aeroport_2 */
Aeroport copy_aeroport(Aeroport aeroport_1, Aeroport aeroport_2)
{
    strcpy(aeroport_2.id, aeroport_1.id);
    strcpy(aeroport_2.country, aeroport_1.country);
    strcpy(aeroport_2.city, aeroport_1.city);
    aeroport_2.num_flights = aeroport_1.num_flights;

    return aeroport_2;
}

/* Função que copia o flight_1 para o flight_2 */
Flight copy_flight(Flight flight_1, Flight flight_2)
{
    strcpy(flight_2.code, flight_1.code);
    strcpy(flight_2.aeroport_out_id, flight_1.aeroport_out_id);
    strcpy(flight_2.aeroport_arrive_id, flight_1.aeroport_arrive_id); 
    flight_2.date.day = flight_1.date.day;
    flight_2.date.month = flight_1.date.month;
    flight_2.date.year = flight_1.date.year;
    flight_2.hour.hours = flight_1.hour.hours;
    flight_2.hour.minutes = flight_1.hour.minutes;
    flight_2.duration.hours = flight_1.duration.hours;
    flight_2.duration.minutes = flight_1.duration.minutes;
    flight_2.max_passengers = flight_1.max_passengers;

    return flight_2;
}

/* Função que dadas duas horas retorna a hora relativa à sua soma */
Time add_time(Time time1, Time time2)
{
    Time sum;

    sum.minutes = time1.minutes + time2.minutes;
    sum.hours = time1.hours + time2.hours;
    sum.hours += sum.minutes / 60;
    sum.minutes %= 60;

    return sum;
}

/* Função que verifica a validade do número de passageiros num voo */
int invalid_capacity(Flight flight)
{
    if (flight.max_passengers >= 10 && flight.max_passengers <= 100)
        return 0;
    return 1;
}

/* Função que verifica a validade da duração de um voo */
int high_fligth_duration(Time duration)
{
    if (duration.hours == 12 && duration.minutes > 0)
        return 1;
    else if (duration.hours > 12)
        return 1;
    return 0;
}

/* Função que verifica se date_1 é no passado relativamente a date_2 */
int past_date(Date date_1, Date date_2)
{
    if (date_1.year < date_2.year)
        return 1;
    if ((date_1.year == date_2.year) && (date_1.month < date_2.month))
        return 1;
    if ((date_1.year == date_2.year) && (date_1.month == date_2.month) && (date_1.day < date_2.day))
        return 1;
    return 0;
}

/* Função que verifica se date_1 é depois de um ano relativamente a date_2 */
int future_date(Date date_1, Date date_2)
{
    if (date_1.year > 2023)
        return 1;
    if (date_1.year - date_2.year > 1)
        return 1;
    if ((date_1.year - date_2.year == 1) && (date_1.month > date_2.month))
        return 1;
    else if ((date_1.year - date_2.year == 1) && (date_1.month == date_2.month) && date_1.day > date_2.day)
        return 1;
    return 0;
}

/* Função que verifica se date_1 já existe num vetor de voos de tamanho max */
int existing_day(Date date_1, Flight flight_1[], int max)
{
    int i;
    for (i = 0; i < max; i++)
    {
        if (date_1.day == flight_1[i].date.day && date_1.month == flight_1[i].date.month && date_1.year == flight_1[i].date.year)
            return 1;
    }

    return 0;
}

/* Função que verifica se IDA pertence ao vetor de aeroportos de tamanho max */
int in_aeroport(char IDA[], Aeroport aeroport[], int max)
{
    int i;
    for (i = 0; i < max; i++)
    {
        if (strcmp(IDA, aeroport[i].id) == 0)
            return 1;
    }
    return 0;
}

/* Função que verifica se dado um código de voo já existe */
int in_flight(char flight_code[], Flight flights_in[], int max)
{
    int i;
    for (i = 0; i < max; i++)
    {
        if (strcmp(flight_code, flights_in[i].code) == 0)
            return 1;
    }
    return 0;
}

/* Função de sort "bubblesort" para strings */
void bubble_sort_strings(Aeroport aeroport_sort[], int max)
{
    Aeroport aux;
    int i, j;

    for (i = 0; i < max - 1; i++)
    {
        for (j = 0; j < max - 1 - i; j++)
        {
            if (strcmp(aeroport_sort[j].id, aeroport_sort[j + 1].id) > 0)
            {
                aux = aeroport_sort[j];
                aeroport_sort[j] = aeroport_sort[j + 1];
                aeroport_sort[j + 1] = aux;
            }
        }
    }
}

/* Função que verifica se duas datas, date_1 e date_2 são no mesmo dia */
int same_day(Date date_1, Date date_2)
{
    if ((date_1.day == date_2.day) && (date_1.month == date_2.month) && (date_1.year == date_2.year))
        return 1;

    return 0;
}

/* Função que verifica se um voo com o mesmo código já existe no mesmo dia */
int same_day_flight(Flight flight, Flight flights_same[], int max)
{
    int i;

    for (i = 0; i < max; i++)
    {
        if (strcmp(flight.code, flights_same[i].code) == 0)
        {
            if (same_day(flights_same[i].date, flight.date))
                return 1;
        }
    }

    return 0;
}

/* Função que verifica se time_2 é no futuro relativamente a time_1 */
int past_hour(Time time_1, Time time_2)
{
    if (time_1.hours < time_2.hours)
        return 1;
    if ((time_1.hours == time_2.hours) && (time_1.minutes < time_2.minutes))
        return 1;
    return 0;
}

/* Função de sort "bubblesort" para datas. O algoritmo utilizado foi o bubblesort por ser estável */
void bubble_sort_dates(Flight flights_sort[], int max)
{
    Flight aux;
    int i, j;

    for (i = 0; i < max - 1; i++)
    {
        for (j = 0; j < max - 1 - i; j++)
        {
            if (past_date(flights_sort[j + 1].date, flights_sort[j].date))
            {
                aux = flights_sort[j];
                flights_sort[j] = flights_sort[j + 1];
                flights_sort[j + 1] = aux;
            }
        }
    }
}

/* Função de sort "bubblesort" para datas. O algoritmo utilizado foi o bubblesort por ser estável*/
void bubble_sort_time(Flight flights_sort[], int max)
{
    Flight aux;
    int i, j;

    for (i = 0; i < max - 1; i++)
    {
        for (j = 0; j < max - 1 - i; j++)
        {
            if ((past_hour(flights_sort[j + 1].hour, flights_sort[j].hour)) && (same_day(flights_sort[j + 1].date, flights_sort[j].date)))
            {
                aux = flights_sort[j];
                flights_sort[j] = flights_sort[j + 1];
                flights_sort[j + 1] = aux;
            }
        }
    }
}

/* Função que adiciona um dia a date caso as horaas de time forem iguais ou superiores a 24 */
Date add_day(Date date, Time time)
{

    if (time.hours >= 24)
    {
        time.hours -= 24;
        date.day = 1;
    }

    if (date.day > days_of_month[date.month - 1])
    {
        date.day = 1;
        date.month += 1;
    }

    if (date.month == 13)
    {
        date.month = 1;
        date.year++;
    }

    return date;
}

/* Função adicona um aeroport ao sistema se este cumprir os requisitos */
void add_aeroport()
{
    int state_aeroport = OUT, i_aeroport;

    scanf("%s%s ", temp_aeroport.id, temp_aeroport.country);

    fgets(temp_aeroport.city, MAX_CITY, stdin);

    temp_aeroport.city[strlen(temp_aeroport.city) - 1] = '\0';

    /* Se o IDA do aeroporto tiver menos que 3 caracteres é inválido */
    if (strlen(temp_aeroport.id) < 3)
                state_aeroport = IN;


    /* Se os caracteres do IDA do aeroporto não forem todos letras maíusculas este é inválido */
    for (i_aeroport = 0; state_aeroport == OUT && i_aeroport < 3; i_aeroport++)
    {
        if (temp_aeroport.id[i_aeroport] < 'A' || temp_aeroport.id[i_aeroport] > 'Z')
            state_aeroport = IN;
    }

    /* Se o IDA do aeroporto for inválido */
    if (state_aeroport)
    {
        puts("invalid airport ID");
    }
    /* Se já tiverem sido criados 40 aeroportos */
    else if (i_a >= MAX_AEROPORT)
    {
        puts("too many airports");
    }
    /* Se o aeroporto já existir */
    else if (in_aeroport(temp_aeroport.id, aeroports, i_a))
    {
        puts("duplicate airport");
    }
    /* Se nenhum erro se verificar, o aeroporto é adicionado ao sistema */
    else
    {
        aeroports[i_a] = copy_aeroport(temp_aeroport, aeroports[i_a]);
        printf("airport %s\n", temp_aeroport.id);
        i_a++;
    }
}

/* Função que lista os aeroportos do sistema por ordem alfabética de IDA */
void list_aeroports_alone()
{
    int i;

    for (i = 0; i < i_a; i++)
    {                
        printf("%s %s %s %d\n", aeroports[i].id, aeroports[i].city, aeroports[i].country, aeroports[i].num_flights);
    }
}

/* Função que lista os aeroportos do sistema correspondentes aos IDA's lidos como argumentos do comando l, por ordem de leitura*/
void list_set_aeroports()
{
    int i;
    char IDA[MAX_ID];
    scanf("%s", IDA);

    if (in_aeroport(IDA, aeroports, i_a))
    {
        for (i = 0; i < i_a; i++)
        {
            if (strcmp(IDA, aeroports[i].id) == 0)
            {
                printf("%s %s %s %d\n", aeroports[i].id, aeroports[i].city, aeroports[i].country, aeroports[i].num_flights); 
            }
        }
    }   
    else
        printf("%s: no such airport ID\n", IDA);

}

/* Função que adiciona um voo ao sistema se este cumprir os requisitos */
void add_flight()
{
    int state_flight = OUT, i_flight = 0, i = 0;
    long unsigned int i_flight_unsigned = 0;

    scanf("%s %s %s %d-%d-%d %d:%d %d:%d %d", temp_flight.code, temp_flight.aeroport_out_id, temp_flight.aeroport_arrive_id, &temp_flight.date.day, &temp_flight.date.month, &temp_flight.date.year, &temp_flight.hour.hours, &temp_flight.hour.minutes, &temp_flight.duration.hours, &temp_flight.duration.minutes, &temp_flight.max_passengers);

    /* Se o tamanho do código do voo for menor que 3 ele automaticamente é inválido */
    if (strlen(temp_flight.code) < 3)
        state_flight = IN;

    /* Se a série de dígitos a seguir às letras começar com um 0, o código do voo é inválido */
    if (state_flight == OUT && temp_flight.code[2] == '0')
            state_flight = IN;

    /* dois for's para verificar a validade do código do voo */

    /* Se os dois primeiros caracteres não forem letras maiúsculas */
    for (i_flight = 0; state_flight == OUT && i_flight < 2  ; i_flight++)
    {
        if (temp_flight.code[i_flight] < 'A' || temp_flight.code[i_flight] > 'Z')
            state_flight = IN;
    }
    /* Se os restantes caracteres do não forem dígitos */
    for (i_flight_unsigned = 2; state_flight == OUT && i_flight_unsigned < strlen(temp_flight.code); i_flight_unsigned++)
    {
        if (!isdigit(temp_flight.code[i_flight_unsigned]))
                        state_flight = IN;
    }
    /* Se o código do voo for inválido */
    if (state_flight)
        puts("invalid flight code");
    /* Se um voo com o mesmo código no mesmo dia já existir */
    else if (in_flight(temp_flight.code, flights, i_v) && same_day_flight(temp_flight, flights, i_v))
    {
        puts("flight already exists");
    }
    /* Se o aeroporto de partida correspondente ao voo não exitir */
    else if (!in_aeroport(temp_flight.aeroport_out_id, aeroports, i_a))
        printf("%s: no such airport ID\n", temp_flight.aeroport_out_id);
    /* Se o aeroporto de chegada correspondente ao voo não exitir */
    else if (!in_aeroport(temp_flight.aeroport_arrive_id, aeroports, i_a))
        printf("%s: no such airport ID\n", temp_flight.aeroport_arrive_id);
    /* Se já tiverem sido criados 30000 voos */
    else if (i_v >= MAX_FLIGHT)
    {
        puts("too many flights");
        i_v--;
    }
    /* Se a data do voo for inválida */
    else if ((past_date(temp_flight.date, date)) || future_date(temp_flight.date, date))
        puts("invalid date");
    /* Se a duração do voo for inválida */
    else if (high_fligth_duration(temp_flight.duration))
        puts("invalid duration");
    /* Se a capacidade do voo for inválida */
    else if (invalid_capacity(temp_flight))
        puts("invalid capacity");

    /* Se nenhum erro se verificar então o voo é adicionado ao sistema */
    else
    {
        flights[i_v] = copy_flight(temp_flight, flights[i_v]);
        for (i = 0; i < i_a; i++)
        {
            if ((strcmp(flights[i_v].aeroport_out_id, aeroports[i].id)) == 0)
                aeroports[i].num_flights++;
        }
        i_v++;
    }
}

/* Função que lista os voos do sistema por ordem de criação */
void list_flights()
{
    int i_flight = 0;

    for (i_flight = 0; i_flight < i_v; i_flight++)
    {
        printf("%s %s %s %02d-%02d-%04d %02d:%02d\n", flights[i_flight].code, flights[i_flight].aeroport_out_id, flights[i_flight].aeroport_arrive_id, flights[i_flight].date.day, flights[i_flight].date.month, flights[i_flight].date.year, flights[i_flight].hour.hours, flights[i_flight].hour.minutes);
    }
}

/* Função que adiciona um voo a um vetor auxiliar de voos que contém os voos correspondentes ao IDA do aeroporto de partida lido */
void add_flight_p()
{
    char IDA[MAX_ID];
    int i;
    scanf("%s", IDA);

    if (in_aeroport(IDA, aeroports, i_a))
    {
        for (i = 0; i < i_v; i++)
        {
            if (strcmp(flights[i].aeroport_out_id, IDA) == 0)
            {
                flights_aux_p[i_p] = copy_flight(flights[i], flights_aux_p[i_p]);
                i_p++;
            }
        }
    }
    else
        printf("%s: no such airport ID\n", IDA);
}

/* Função que lista os voos com partida de um certo aeroporto por ordem de data e hora de partida, da mais antiga para a mais recente*/
void list_flights_p()
{
    int i;
    for (i = 0; i < i_p; i++)
        printf("%s %s %02d-%02d-%04d %02d:%02d\n", flights_aux_p[i].code, flights_aux_p[i].aeroport_arrive_id, flights_aux_p[i].date.day, flights_aux_p[i].date.month, flights_aux_p[i].date.year, flights_aux_p[i].hour.hours, flights_aux_p[i].hour.minutes);

    i_p = 0;
}


/* Função que adiciona um voo a um vetor auxiliar de voos que contém os voos correspondentes ao IDA do aeroporto de chegada lido */
void add_flight_c()
{
    char IDA[MAX_ID];
    int i;

    scanf("%s", IDA);

    if (in_aeroport(IDA, aeroports, i_a))
    {
        for (i = 0; i < i_v; i++)
        {
            if (strcmp(flights[i].aeroport_arrive_id, IDA) == 0)
            {
            
                flights_aux_c[i_c] = copy_flight(flights[i], flights_aux_c[i_c]);                                                                                                   
                flights_aux_c[i_c].hour = add_time(flights_aux_c[i_c].hour, flights_aux_c[i_c].duration);

                if (flights_aux_c[i_c].hour.hours >= 24)
                {
                    flights_aux_c[i_c].date.day += 1;
                    flights_aux_c[i_c].hour.hours -= 24;
                }

                flights_aux_c[i_c].date = add_day(flights_aux_c[i_c].date, flights_aux_c[i_c].hour);

                i_c++;
            }
        }
    }
    else
        printf("%s: no such airport ID\n", IDA);
}

/* Função que lista os voos com partida de um certo aeroporto por ordem de data e hora de chegada, da mais antiga para a mais recente*/
void list_flights_c()
{
    int i;

    for (i = 0; i < i_c; i++)
        printf("%s %s %02d-%02d-%04d %02d:%02d\n", flights_aux_c[i].code, flights_aux_c[i].aeroport_out_id, flights_aux_c[i].date.day, flights_aux_c[i].date.month, flights_aux_c[i].date.year, flights_aux_c[i].hour.hours, flights_aux_c[i].hour.minutes);

    i_c = 0;
}

/* Função que altera a data do sistema se esta for válida */
void change_date()
{
    scanf("%d-%d-%d", &new_date.day, &new_date.month, &new_date.year);


    /* Se a data for no passado ou mais de 1 ano no futuro é inválida */
    if (past_date(new_date, date) || future_date(new_date, date))
        puts("invalid date");
    else
    {
        printf("%02d-%02d-%04d\n", new_date.day, new_date.month, new_date.year);

        date.day = new_date.day;
        date.month = new_date.month;
        date.year = new_date.year;
    }
}

/*função main*/
int main()
{
    /* Definição de variáveis */

    char command, next_char;

    /* ciclo que lê o input do stdinput correspondente aos comandos do sistema.
    Utiliza-se um ciclo do while, para garantir que este é executado pelo menos uma vez. */
    do
    {
        command = getchar();
        
        /* FALTA POR ISTO NUMA FUNÇÃO BONITA DE RESTO ESTÁ TUDO AQUI */

        /* comando 'a' */
        if (command == 'a')
        {        
            /* Adiciona um novo aeroporto ao sistema */
            add_aeroport();
        }
        /* comando 'l' */
        else if (command == 'l')
        {
            /* Organiza os aeroportos por ordem alfabética de IDA */
            bubble_sort_strings(aeroports, i_a);

            /* Lê o próximo caracter para saber se o comando foi invocado com argumentos */
            next_char = getchar();

            /* Se sim, lista todos os aeroportos por ordem alfabética de IDA */
            if (next_char == '\n')
            {
                list_aeroports_alone();
            }

            /* Se sim, listar todos os aeroportos do sistema correspondentes aos IDA's lidos como argumentos do comando, por ordem de leitura*/
            else
            {
                while (next_char != '\n')
                {                 
                    list_set_aeroports();

                    next_char = getchar();
                }
            }
        }
        /* comando 'v' */
        else if (command == 'v')    
        {
            /* Adiciona um novo voo ao sitema */
            if ((next_char = getchar()) != '\n')
            {
                add_flight();
            }
            /* Se não, lista os voos por ordem de criação */
            else
            {
                list_flights();
            }
        }
        /* comando 'p' */
        else if (command == 'p')
        {
            /* Adiciona voos ao vetor auxiliar "fligts_aux_p" */
            add_flight_p();            

            /* Organiza esses voos por data e hora (do mais antigo para o mais recente) */
            bubble_sort_dates(flights_aux_p, i_p);

            bubble_sort_time(flights_aux_p, i_p);

            /* E listá-los */
            list_flights_p();

        }
        /* comando 'c' */
        else if (command == 'c')
        {
            /* Adiciona voos ao vetor auxiliar "fligts_aux_c" */
            add_flight_c();

            /* Organiza esses voos por data e hora (do mais antigo para o mais recente) */
            bubble_sort_dates(flights_aux_c, i_c);

            bubble_sort_time(flights_aux_c, i_c);

            /* E listá-los */
            list_flights_c();
        }
        /* comando 't' */
        else if (command == 't')
        {
            /* Altera a data do sistema */
            change_date();
        }
    /* Se o comando lido for 'q' então o programa é terminado */
    } while (command != 'q');

    return 0;
}
