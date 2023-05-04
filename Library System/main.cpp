#include <iostream>
#include <algorithm>
using namespace std;
const int MAX_BOOKS=10;
const int MAX_USERS=10;

struct Book
{
    string name;
    int id;
    int total_quantaty;
    int total_borrowed;

    Books()
    {
        total_borrowed=total_quantaty=0;
        name="";
        id=-1;
    }

    void read()
    {
        cout<<"Enter the id , name , total quantity : ";
        cin>>id>>name>>total_quantaty;
        total_borrowed=0;
    }

    bool has_prefix(string prefix)
    {
        if(prefix.size()>name.size())
            return false;

        for(int i=0;i<prefix.size();i++)
        {
              if(prefix[i] !=name[i])
              return false;
        }
        return true;
    }

    bool borrow()
    {
        if(total_quantaty<=total_borrowed)
            return false;

            ++total_borrowed;
        return true;
    }

    void return_copy()
    {
        --total_borrowed;
    }

    void print()
    {
        cout<<"id = "<<id<<" name= "<<name<<" total quantity "
        <<total_quantaty<<" total borrowed "<< total_borrowed
        <<"\n";
    }
};

struct User
{
    string name;
    int id;
    int borrowed_books_ids[MAX_BOOKS];
    int len;

    User()
    {
        name="";
        len=0;
        id=-1;
    }

   bool is_borrowed(int book_id)
    {
        for(int i=0;i<len;++i)
        {
           if(book_id=borrowed_books_ids[i])
            return true;
        }
        return false;
    }

    void read()
    {
        cout<<"Enter name, id: ";
        cin>>name>>id;
    }

    void borrow(int book_id)
    {
        borrowed_books_ids[len++]=book_id;
    }

    void return_copy(int book_id)
    {
        for(int i=0;i<len;i++)
        {
            if(borrowed_books_ids[i]==book_id)
            {
                for(int j=i+1;j<len;j++)
                    borrowed_books_ids[j-1]=borrowed_books_ids[j];

                    --len;
                    break;
            }
        }
    }


    void print()
    {
        sort(borrowed_books_ids,borrowed_books_ids+len);

        cout<<"user "<<name<<" id "<<id<<" borrowed books ids: ";

        for(int i=0;i<len;i++)
            cout<<borrowed_books_ids[i]<<" ";
        cout<<"\n";
    }

};
 bool cmp_book_by_name(Book &a, Book &b)
 {
  return a.name < b.name;
 }

 bool cmp_book_by_id(Book &a, Book &b)
 {
     return a.id < b.id;
 }
struct Library_System
{
    int total_books;
    Book book[MAX_BOOKS];
    int total_users;
    User user[MAX_USERS];

   Library_System()
   {
       total_books=total_users=0;
   }
   void run()
   {


       while(true)
       {
         int choice=menu();
           if(choice==1)
            add_book();
           else if(choice==2)
            search_books_by_prefix();
           else if(choice==3)
            print_who_borrowed_book_by_name();
           else if(choice==4)
            print_library_by_id();
           else if(choice==5)
            print_library_by_name();
           else if(choice==6)
            add_user();
           else if(choice==7)
            user_borrow_book();
           else if(choice==8)
            user_return_book();
           else if(choice==9)
            print_users();
           else
            break;
       }
   }
   int menu()
   {
       int choice=-1;

       while(choice==-1)
       {
          cout<<"\nLibrary System\n";
          cout<<"1) add book\n";
          cout<<"2) search book by prefix\n";
          cout<<"3) print who borrowed book by name\n";
          cout<<"4) print library by id\n";
          cout<<"5) print library by name\n";
          cout<<"6) add user\n";
          cout<<"7) user borrow book\n";
          cout<<"8) user return book\n";
          cout<<"9) print users\n";
          cout<<"10) Exit\n";

          cout<<"\nEnter your choice [1-10]: ";
          cin>>choice;

          if(!(choice<=10&& choice>=1))
          {
              cout<<"Invalid choice. Try again\n";
              choice=-1;
          }
       }
      return choice;
   }

   void add_book()
   {
       book[total_books++].read();
   }

   void search_books_by_prefix()
   {
       string prefix;
       cout<<"Enter the prefix: ";
       cin>>prefix;

       int cnt=0;
       for(int i=0;i<total_books;i++)
       {
           if(book[i].has_prefix(prefix))
            cout<<book[i].name<<"\n";
            cnt++;
       }

       if(!cnt)
        cout<<"No books with such prefix\n";
   }
   int find_book_idx_by_name(string name)
   {
       for(int idx=0;idx<total_books;idx++)
       {
           if(book[idx].name==name)
            return idx;
       }
       return -1;
   }

   int find_user_idx_by_name(string name)
   {
        for(int idx=0;idx<total_users;idx++)
        {
          if(user[idx].name==name)
                return idx;
        }
        return -1;
   }
   void print_who_borrowed_book_by_name()
   {
       string book_name;
       cout<<"Enter the book name: ";
       cin>>book_name;

       int book_idx=find_book_idx_by_name(book_name);

       if(book_idx==-1)
       {
           cout<<"Invalid book name.\n";
           return;
       }
      int book_id=book[book_idx].id;

      if(book[book_idx].total_borrowed==0)
      {
          cout<<"NO borrowed copies\n";
          return;
      }

      for(int i=0;i<total_users;i++)
      {
          if(user[i].is_borrowed(book_id))
            cout<<user[i].name<<"\n";
      }

   }
   void print_library_by_id()
   {
       sort(book,book+total_books,cmp_book_by_id);

       cout<<"\n";
     for(int i=0;i<total_books;i++)
     {
        book[i].print();
     }
   }

   void print_library_by_name()
   {
      sort(book,book+total_books,cmp_book_by_name);

      cout<<"\n";
      for(int i=0;i<total_books;++i)
            book[i].print();
   }

   void add_user()
   {
       user[total_users++].read();
   }
   bool read_user_name_and_book_name(int &user_idx,int &book_idx,int trail=3)
   {
       string user_name;
       string book_name;

       while(trail--)
       {
           cout<<"Enter user name and book name: ";
           cin>>user_name>>book_name;

          user_idx=find_user_idx_by_name(user_name);

          if(user_idx==-1)
          {
              cout<<"Invalid user name,try again\n";
              continue;
          }


         book_idx=find_book_idx_by_name(book_name);
         if(book_idx==-1)
         {
             cout<<"Invalid book name,try again\n";
             continue;
         }

           return true;
       }
       cout<<"you did several trials! try later.";
       return false;
   }
   void user_borrow_book()
   {
       int user_idx;
       int book_idx;

     if(!read_user_name_and_book_name(user_idx,book_idx))
        return;

     int user_id=user[user_idx].id;
     int book_id=book[book_idx].id;

    if(!book[book_idx].borrow())
        cout<<"No more copies available right now\n";
    else
       user[user_idx].borrow(book_id);
   }

   void user_return_book()
   {
       int user_idx,book_idx;

       if(!read_user_name_and_book_name(user_idx,book_idx))
        return;

       int book_id=book[book_idx].id;

       if(!user[user_idx].is_borrowed(book_id))
       {
           cout<<"This user not borrow this book\n";
           return;
       }

       book[book_idx].return_copy();
       user[user_idx].return_copy(book_id);
   }

   void print_users()
   {
       cout<<"\n";

       for(int i=0;i<total_users;i++)
            user[i].print();
   }
};
int main()
{
    Library_System library;

    library.run();
    return 0;
}
