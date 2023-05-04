#include<iostream>
using namespace std;

// Global variables
const int MAX_SPECIALIZATION = 20;
const int MAX_QUEUE = 5;

string names[MAX_SPECIALIZATION+1][MAX_QUEUE+1];
int status[MAX_SPECIALIZATION+1][MAX_QUEUE+1];
int queue_length[MAX_SPECIALIZATION+1];
int Show_menu()
{
  int choice=-1;

    while(choice==-1)
    {

    cout<<"\nEnter the choice :\n"<<
    "1- Add new patients\n"<<
    "2- Print all patients\n"<<
    "3- Get next patients\n"<<
    "4- Exist\n";

    cin>>choice;

    if(!(choice>=1&&choice<=4))
    {
        cout<<"The choice is not correct,Please enter Invalid choice\n";
        choice =-1;
    }
 }
 return choice;
}

void shift_left(int spe,string name[],int state[])
{
   int pos=queue_length[spe];
  for(int i=0;i<pos;i++)
  {
      name[i]=name[i+1];
      state[i]=state[i+1];
  }
  queue_length[spe]--;
}
void shift_right(int spe,string name[], int state[])
{
    int pos=queue_length[spe];

    for(int i=pos-1;i>=0;i--)
    {
        name[i+1]=name[i];
        state[i+1]=state[i];
    }
    queue_length[spe]++;
}


void add_Patient()
{
    int spe;
    string name;
    int state;

    cout<<"Enter the specializations ,name , states : ";

    cin>>spe>>name>>state;

    int pos=queue_length[spe];

    if(pos>=MAX_QUEUE)
    {
     cout<<"Sorry this specializations is full \n";
     return;
    }
     if(state==1)
     {
        shift_right(spe,names[spe],status[spe]);
         names[spe][0]=name;
        status[spe][0]=state;
        return;

     }

    names[spe][pos]=name;
    status[spe][pos]=state;
    queue_length[spe]++;

}
void print_patient(int spe,string name[],int state[])
{
    int pos=queue_length[spe];
    if(pos==0)
        return;

    cout<<"There are "<< pos<<" patients in specialization "<<spe<<"\n";

   for(int i=0;i<pos;i++)
   {
       cout<<name[i];

       if( state[i])
        cout<<" urgent\n";
        else
        cout<<" regular\n";
   }
   cout<<"\n";
}
void print_patients()
{
    for(int spe=0;spe<MAX_SPECIALIZATION;spe++)
    {
        print_patient(spe,names[spe],status[spe]);
    }
}

void get_next_patient()
{
    int spe;
    cout<<"Enter the specialization : ";
    cin>>spe;

    int pos=queue_length[spe];
    if(pos==0)
    {
        cout<<"No patient now ,have rest, Dr\n";
        return;
    }
    cout<<names[spe][0]<<", Please go to the Dr\n";
    shift_left(spe,names[spe],status[spe]);
}

void Hospital_System()
{
    while(true)
    {
        int choice=Show_menu();

        if(choice==1)
              add_Patient();
         else if(choice==2)
               print_patients();
         else if(choice==3)
             get_next_patient();
         else
            break;

        }
    }

int main()
{
     Hospital_System();
    return 0;
}
