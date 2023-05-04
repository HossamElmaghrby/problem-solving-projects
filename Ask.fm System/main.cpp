#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>


using namespace std;

vector<string> ReadFilelines(string path)
{
 vector<string> lines;

 fstream file_handler(path.c_str());

 if(file_handler.fail())
 {
     cout<<"\n\nERROR: can't open the file\n\n";
     return lines;
 }
 string line;

 while(getline(file_handler,line))
 {
     if(line.size()==0)
        continue;

     lines.push_back(line);
 }

 file_handler.close();

 return lines;
}

void WriteFilelines(string path,vector<string>lines,bool append=true)
{
    auto status=ios::in|ios::out|ios::app;

    if(!append)
    status=ios::in|ios::out|ios::trunc;

    fstream file_handler(path.c_str(),status);

    if(file_handler.fail())
    {
        cout<<"\n\nERROR: can't open the file\n\n";
        return;
    }

    for(auto &line:lines)
        file_handler<<line<<"\n";

    file_handler.close();


}

int ToInt(string str)
{
    istringstream iss(str);
    int num;
    iss>>num;

    return num;
}
vector<string> SplitString(string s,string delimiter=",")
{
    vector<string> strs;

    int pos=0;
    string Substr;

    while((pos=(int)s.find(delimiter))!=-1)
    {
      Substr=s.substr(0,pos);
      strs.push_back(Substr);
      s.erase(0,pos+delimiter.length());
    }
    strs.push_back(s);
    return strs;
}
int ReadInt(int low, int high)
{
    cout<<"Enter the choice between["<<low<<" , "<<high<<"]: ";

    int choice;
    cin>>choice;

    if(low<=choice&&choice<=high)
        return choice;

        cout<<"ERROR: invalid number... Try again\n";
    return ReadInt(low,high);
}
int showReadMenu(vector<string> menu)
{

    cout<<"\n menu \n";
    for(int i=0;i<(int)menu.size();i++)
    {
          cout<<"\t"<<i+1<<":"<<menu[i]<<"\n";
    }
    return ReadInt(1,menu.size());
}


struct Question
{
    int question_id;
    int from_user_id;
    int to_user_id;
    int is_anonymous_question;
    int parent_question_id;
    string question_text;
    string answer_text;

    Question()
    {
        int question_id=from_user_id=to_user_id=is_anonymous_question=parent_question_id=-1;
    }

    Question(string line)
    {
        vector<string> Substr=SplitString(line);
        question_id=ToInt(Substr[0]);
        parent_question_id=ToInt(Substr[1]);
        from_user_id=ToInt(Substr[2]);
        to_user_id=ToInt(Substr[3]);
        is_anonymous_question=ToInt(Substr[4]);
        question_text=Substr[5];
        answer_text=Substr[6];

    }
    string ToString()
    {
        ostringstream oss;

        oss<<question_id<<","<<parent_question_id<<","<<from_user_id<<","<<to_user_id<<","<<is_anonymous_question<<","<<question_text<<","<<answer_text;
        return oss.str();
    }
    void PrintToQuestion()
    {
        string prefix="";

        if(parent_question_id!=-1)
            prefix="\tThread: ";

       cout<<prefix<<" Question id("<<question_id<<")";

       if(!is_anonymous_question)
        cout<<"from user id ("<<from_user_id<<")";

       cout<<"\t Question: "<<question_text<<"\n";

       if(answer_text!="")
           cout<<prefix<< "\tAnswer: "<<answer_text<<"\n";

           cout<<"\n";

    }
    void PrintFromQuestions()
    {
       cout<<"Question Id ("<<question_id<<")";

       if(!is_anonymous_question)
         cout<<" !AQ";

       cout<<" to user id("<<to_user_id<<")";

       cout<<"\t Question: "<<question_text;

       if(answer_text!="")
        cout<<"\tAnswer: "<<answer_text<<"\n";
        else
            cout<<"\tNOT Answered YET\n";
    }
    void PrintFeedQuestion()
    {
        if(parent_question_id!=-1)
            cout<<"Thread parent question ID (" <<parent_question_id<<") ";

        cout<<"Question Id ("<<question_id<<")";

        if(!is_anonymous_question)
         cout<<" From suer id("<<from_user_id<<")";

         cout<<"To user id("<<to_user_id<<")";

         cout<<"\t Question: "<<question_text<<"\n";

         if(answer_text!="")
            cout<<"\tAnswer: "<<answer_text<<"\n";
    }
};

struct User
{
    int user_id;
    string name;
    string user_name;
    string email;
    string password;
    int allow_anonymous_questions;

    vector<int> question_id_from_me;

    map<int ,vector<int>> questionid_questionidsthread_to_map;

    User()
    {
        user_id=allow_anonymous_questions=-1;
    }
    User(string line)
     {
         vector<string> Substr=SplitString(line);

         user_id=ToInt(Substr[0]);
         user_name=Substr[1];
         password=Substr[2];
         name=Substr[3];
         email=Substr[4];
         allow_anonymous_questions=ToInt(Substr[5]);
     }
   string ToString()
   {
      ostringstream oss;

      oss<<user_id<<","<<user_name<<","<<password<<","<<name<<","<<email<<","<<allow_anonymous_questions;

      return oss.str();
   }
};

struct QuestionsManager
{
    map<int,Question> questionid_questionobject_map;

    map<int ,vector<int>> questionid_questionidsThread_to_map;

    int last_id;

    QuestionManager()
    {
        last_id=-1;
    }
    void LoadDatabase()
    {
        last_id=0;
        questionid_questionidsThread_to_map.clear();
        questionid_questionobject_map.clear();

        vector<string> lines=ReadFilelines("questions.txt");

        for(auto &line:lines)
        {
            Question question(line);
            last_id=max(question.question_id,last_id);

            questionid_questionobject_map[question.question_id]=question;

            if(question.parent_question_id==-1)
            questionid_questionidsThread_to_map[question.question_id].push_back(question.question_id);
            else
            questionid_questionidsThread_to_map[question.parent_question_id].push_back(question.question_id);
        }
    }

    void FillUserQestions(User &user)
    {
        user.questionid_questionidsthread_to_map.clear();
        user.question_id_from_me.clear();

        for(auto &pair:questionid_questionidsThread_to_map)
        {
            for(auto &question_id:pair.second)
            {
                Question &question=questionid_questionobject_map[question_id];

                if(question.from_user_id==user.user_id)
                    user.question_id_from_me.push_back(question_id);

                if(question.to_user_id==user.user_id)
                {
                    if(question.parent_question_id==-1)
                        user.questionid_questionidsthread_to_map[question.question_id].push_back(question.question_id);
                    else
                        user.questionid_questionidsthread_to_map[question.parent_question_id].push_back(question.question_id);
                }
            }
        }
    }

    int ReadQuestionAny(User &user)
     {
         int question_id;
         cout<<"Enter Question id or -1 to cancel: ";
         cin>>question_id;

         if(question_id==-1)
            return -1;

         if(!questionid_questionobject_map.count(question_id))
         {
            cout<<"\nERROR: No question with such ID .Try again\n\n";
            return ReadQuestionAny(user);
         }
         Question &question=questionid_questionobject_map[question_id];

         if(question.to_user_id!=user.user_id)
         {
             cout<<"\nERROR: Invalid question ID.Try again\n\n";
             return ReadQuestionAny(user);

         }
         return question_id;
     }
    int ReadQuestionIdThread()
    {
        int question_id;
        cout<<"For thread question : Enter id or -1 for new question: ";
        cin>>question_id;

        if(question_id==-1)
            return -1;

        if(!questionid_questionidsThread_to_map.count(question_id))
        {
            cout<<"No thread question with such ID. Try again\n";
            return ReadQuestionIdThread();
        }
        return question_id;
    }

    void PrintUserToQuestions(User &user)
    {
        cout<<"\n";
        if(user.questionid_questionidsthread_to_map.size()==0)
        {
            cout<<"No questions";
            return;
        }

        for(auto &pair:user.questionid_questionidsthread_to_map)
        {
            for(auto &question_id:pair.second)
            {
                Question &question =questionid_questionobject_map[question_id];
                question.PrintToQuestion();
            }
        }
    }

    void PrintUserFromQuestions(User &user)
    {
       if(user.question_id_from_me.size()==0)
       {
         cout<<"No questions";
         return;
       }
       for(auto question_id:user.question_id_from_me)
       {
           Question &question=questionid_questionobject_map[question_id];
           question.PrintFromQuestions();
       }
    }

    void AnswerQuestion(User &user)
    {
         int question_id=ReadQuestionAny(user);

         if(question_id==-1)
            return;
          Question &question =questionid_questionobject_map[question_id];

          question.PrintToQuestion();
          if(question.answer_text!="")
            cout<<"\nWarning: Already answered. Answer will be updated\n\n";
          cout<<"Enter answer: ";
           cin.ignore();
          getline(cin,question.answer_text);


    }

    void DeleteQuestion(User &user)
    {

       int question_id=ReadQuestionAny(user);

       if(question_id==-1)
            return;

       vector<int> ids_to_remove;

       if(questionid_questionidsThread_to_map.count(question_id))
       {
           ids_to_remove=questionid_questionidsThread_to_map[question_id];
           questionid_questionidsThread_to_map.erase(question_id);
       }
       else
       {
           ids_to_remove.push_back(question_id);

           for(auto &pair:questionid_questionidsThread_to_map)
           {
               vector<int>&vec=pair.second;

               for(int i=0;i<vec.size();i++)
               {
                   if(vec[i]==question_id)
                   {
                    vec.erase(vec.begin()+i);
                   break;
                   }
               }
           }
       }
        for(auto id:ids_to_remove)
        {
            questionid_questionobject_map.erase(id);
        }
    }
    void AskQuestion(User &user,pair<int,int>to_user_pair)
    {
        Question question;
        if(!to_user_pair.second)
        {
            cout<<"Note: Anonymous questions are not allowed for this user\n";
             question.is_anonymous_question=0;
        }
        else
        {
            cout<<"is anonymous question?: (0 or 1): ";
            cin>>question.is_anonymous_question;
        }

        question.parent_question_id=ReadQuestionIdThread();

        cout<<"Enter question text: ";
        cin.ignore();
        getline(cin,question.question_text);


        question.from_user_id=user.user_id;
        question.to_user_id=to_user_pair.first;

        question.question_id=++last_id;

        questionid_questionobject_map[question.question_id]=question;

        if(question.parent_question_id==-1)
            questionid_questionidsThread_to_map[question.question_id].push_back(question.question_id);
        else
            questionid_questionidsThread_to_map[question.parent_question_id].push_back(question.question_id);
    }
    void ListFeed()
    {
        for(auto &pair: questionid_questionobject_map)
        {
            Question &question=pair.second;
            if(question.answer_text=="")
                continue;
            question.PrintFeedQuestion();
        }
    }
    void UpdataDatabase()
    {
        vector<string> lines;

        for(auto &pair:questionid_questionobject_map)
          lines.push_back(pair.second.ToString());

          WriteFilelines("questions.txt",lines,false);
    }
};


struct UsersManager
{
    map<string,User> username_userobject_map;
    User current_user;
    int last_id;

    UserManager()
    {
        last_id=-1;
    }

   void AccessSystem()
   {
       int choice=showReadMenu({"Login" ,"Sign Up"});

       if(choice==1)
        DoLogin();
       else
        DoSignUp();
   }

   void DoLogin()
   {
       LoadDatabase();

       while(true)
       {
           cout<<"Enter user name & password: ";
           cin>>current_user.user_name>>current_user.password;

           if(!username_userobject_map.count(current_user.user_name))
              {
                  cout<<"\nInvalid user name or password. Try again\n\n";
                  continue;
              }
              User user=username_userobject_map[current_user.user_name];

              if(current_user.password!=user.password)
               {
                   cout<<"\nInvalid user name or password. Try again\n\n";
                   continue;
               }

               current_user=user;
               break;
       }
   }

   void DoSignUp()
   {
     while(true)
     {
         cout<<"Enter user name.(No spaces): ";
         cin>>current_user.user_name;

         if(username_userobject_map.count(current_user.user_name))
             cout<<"Already used. Try again \n";
          else
            break;
     }

     cout<<"Enter password: ";
     cin>>current_user.password;

     cout<<"Enter name: ";
     cin>>current_user.name;

     cout<<"Enter email: ";
     cin>>current_user.email;

     cout<<"Allow anonymous questions?(0 or 1): ";
     cin>>current_user.allow_anonymous_questions;

     current_user.user_id=++last_id;

     username_userobject_map[current_user.user_name]=current_user;

     UpdateDatapase(current_user);
   }
    pair<int,int>ReadUserId()
    {
        int user_id;
        cout<<"Enter User id or -1 to cancel: ";
        cin>>user_id;

        if(user_id==-1)
            return make_pair(-1,-1);

        for(auto &pair:username_userobject_map)
        {
            if(pair.second.user_id==user_id)
                return make_pair(user_id,pair.second.allow_anonymous_questions);
        }
        cout<<"Invalid User ID,Try again\n";
        return ReadUserId();
    }
    void ListUserNameIds()
    {
        for(auto &pair:username_userobject_map)
        cout<<"ID: "<<pair.second.user_id<<"\t\tName: "<<pair.second.name<<"\n";
    }

    void LoadDatabase()
    {
        last_id=0;
        username_userobject_map.clear();

        vector<string> lines= ReadFilelines("users.txt");
        for(auto &line:lines)
        {
            User user(line);

            username_userobject_map[user.user_name]=user;
            last_id=max(user.user_id,last_id);
        }
    }

    void UpdateDatapase(User &user)
    {
        string line=user.ToString();
        vector<string> lines({line});
        WriteFilelines("users.txt",lines);
    }
};

struct AskMeSystem
{
    UsersManager users_manager;
    QuestionsManager questions_manager;

    void LoadDatabase(bool fill_user_questions=false)
    {
        users_manager.LoadDatabase();
        questions_manager.LoadDatabase();

        if(fill_user_questions)
        {
            questions_manager.FillUserQestions(users_manager.current_user);
        }
    }

    void run()
    {
        LoadDatabase();
        users_manager.AccessSystem();
        questions_manager.FillUserQestions(users_manager.current_user);
       vector<string> menu;

       menu.push_back("Print Questions To Me");
       menu.push_back("Print Question From Me");
       menu.push_back("Answer Question");
       menu.push_back("Delete Question");
       menu.push_back("Ask Question");
       menu.push_back("List System Users");
       menu.push_back("Feed");
       menu.push_back("Logout");

       while(true)
       {
           int choice=showReadMenu(menu);

           if(choice==1)
            questions_manager.PrintUserToQuestions(users_manager.current_user);
           else if(choice==2)
            questions_manager.PrintUserFromQuestions(users_manager.current_user);
           else if(choice==3)
           {
              questions_manager.AnswerQuestion(users_manager.current_user);
              questions_manager.UpdataDatabase();
           }
           else if(choice==4)
           {
               questions_manager.DeleteQuestion(users_manager.current_user);
               questions_manager.FillUserQestions(users_manager.current_user);
               questions_manager.UpdataDatabase();
           }
           else if(choice==5)
           {
            pair<int,int> to_user_pair=users_manager.ReadUserId();

            if(to_user_pair.first!=-1)
            {
              questions_manager.AskQuestion(users_manager.current_user,to_user_pair);
              questions_manager.UpdataDatabase();
            }
           }
           else if(choice==6)
             users_manager.ListUserNameIds();
           else if(choice==7)
              questions_manager.ListFeed();
            else
                break;
       }
       run();
    }
};


int main()
{
    AskMeSystem service;
    service.run();
    return 0;
}
