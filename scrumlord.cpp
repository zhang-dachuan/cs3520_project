#include <vector>
#include <string>
#include <iostream>

using namespace std;

//forward declaration of classes to avoid cyclic dependency
class Sprint;

class Issue;

class Project;

class User;

class Issue {
public:
    int ID;
    string desc;
    string type;
    //set this when actually creating the issue inside a project
    Project *proj;
    //issues have people assigned to them, perms are handled at project level
    vector<User *> assignees;
    string priority;
    double status;

    Issue(int ID, string desc, string type, string priority) : ID(ID), desc(desc), type(type), priority(priority) {}

    Issue(int ID, string desc, string type, vector<User *> assignees, string priority) : ID(ID), desc(desc), type(type),
                                                                                         assignees(assignees),
                                                                                         priority(priority) {}

    ~Issue() {}
};

class Sprint {
public:
    vector<Issue *> issues;
    int timeframe;

    //potentially not user specific to access level since sprints contain multiple issues
    //vector<*User> assignees;
    //Sprint(vector<*Issue> issues, int timeframe): issues(issues),timeframe(timeframe){}
    Sprint(vector<Issue *> issues_, int timeframe) : issues(issues_), timeframe(timeframe) {}

    Sprint() {}
    ~Sprint() {}
};

class Project {
public:
    vector<Issue *> done;
    vector<Sprint *> in_progress;
    vector<Issue *> todo;
    //user pointer and string of permissions {mem,led,own}
    vector<pair<User *, string>> assignees;

    string name;
    string due;
    string desc;

    //rather, if you want to add things to a project, do it after you establish it like you would irl
    Project(string name, string due, string desc) : name(name), due(due), desc(desc) {}

    Project() {}//bare dec
    ~Project() {}
    //consider lots of inits or also consider only 2
};

class User {
public:
    string username;
    string role;
    //a user might not need perms whatsoever on their own, just projects.

    //vector<pair<string, Project>> perms;
    vector<Project *> projs;

    User(string username) : username(username) {}
    ~User() {}


};

void displayis(Issue* i){
    cout<<"\tissue ID: "<<i->ID<<endl;
    cout<<"\tdesc: "<<i->desc<<endl;
    cout<<"\ttype: "<<i->type<<endl;
}


void clear_cli() {
    //'erase in display' ansi code
    cout << "\x1B[2J\x1B[H";
}

//more to come
enum {
    LOGIN, MENU, CUR_STAT, PROJS, CREATE_PROJ, CREATE_ISSUE, CREATE_SPRINT, EXIT
};

int main() {
    clear_cli();
    //data persistence -> load everything into these vectors right at the beginning
    //big vectors that contain everything are vectors of POINTERS--dereference when accessing.
    vector<User *> users;
    vector<Project *> projects;
    vector<Issue *> issues;
    vector<Sprint *> sprints;
    User *current_user = nullptr;
    int state = LOGIN;
    bool exit = false;
    int count =0;
    while (!exit) {
        switch (state) {
            //login state handles setting curr
            case LOGIN: {
                cout << "Enter the number corresponding to intended action." << endl;
                cout << "[0]: Exit\n[1]: Register new user\nOr, enter your username to log in." << endl;
                string in;
                getline(cin, in);
                if (in == "0") { state = EXIT; }
                else if (in == "1") {
                    while (true) {
                        bool again = false;
                        cout << "Enter new username, or [0] to return." << endl;
                        string new_usr;
                        getline(cin, new_usr);
                        if (new_usr == "0") {
                            clear_cli();
                            break;
                        }
                        if (users.size() != 0) {
                            for (int i = 0; i < users.size(); i++) {
                                if ((*users[i]).username == new_usr) {
                                    clear_cli();
                                    cout << "Username is already taken." << endl;
                                    again = true;
                                    break;
                                }
                            }
                        }
                        if (!again) {
                            users.push_back(new User(new_usr));
                            clear_cli();
                            cout << "User created." << endl;
                            break;
                        }
                    }
                } else {
                    if (users.size() == 0) {
                        clear_cli();
                        cout << "You must register a user." << endl;
                    } else {
                        for (int i = 0; i < users.size(); i++) {
                            if (in == (*users[i]).username) {
                                current_user = users[i];
                                state = MENU;
                                break;
                            } else if (i == users.size() - 1) {
                                clear_cli();
                                cout << "User not found." << endl;
                            }
                        }
                    }
                }
                break;
            }
            case MENU: {
                //keep this format for all option based sections
                clear_cli();
                cout << "Welcome, " << current_user->username << "." << endl;
                cout << "Enter the number corresponding to intended action." << endl;
                cout << "[0]: Exit\n[1]: Current status\n[2]: View project(s)\n[3]: Create new project\n[4]: Logout"
                     << endl;
                string in;
                getline(cin, in);
                if (in == "0") {
                    state = EXIT;
                } else if (in == "1") {
                    clear_cli();
                    state = CUR_STAT;
                } else if (in == "3") {
                    clear_cli();
                    state = CREATE_PROJ;
                } else if (in == "2") {
                    clear_cli();
                    state = PROJS;
                } else if (in == "4") {
                    clear_cli();
                    cout << "Successfully logged out.\n";
                    current_user = 0;
                    state = LOGIN;
                }
                break;
            }
                //these 3 cases below are essentially where all the points lie, check rubric and implement what you can to
                //farm maximum points. All the base level/layout/fundamentals should be done now, just feature implementation
                //and system if we choose to do that
                //good luck!
            case CUR_STAT: {
                //supposed to display what USER has to do
                //use project list to check for projects user is attached to, check sprints/issues via project
                //and then show all assigned issues/sprints that user is attached to together

                //iterate over the projects and see if the use is involved in the issues inside of it
                //if they are, then display the issue

                //this is the work YOU have to do, meaning it only really works for those assigned jobs
                //people like project owners manage the project but do not actually do the work themselves hence have
                //an empty current status unless they add themselves to an issue.
                //we have chosen for current status to only display issues in sprints that user is assigned to,
                //as only sprints are current.
                for (auto project: current_user->projs) {
                    for (auto sp: project->in_progress) {
                        for(auto is: sp->issues){
                            for(auto us: is->assignees){
                                if(us->username==current_user->username){
                                    displayis(is);
                                }
                            }
                        }
                    }
                }
                cout<<"\n[0]: Return\n";
                string rt;
                cin >> rt;
                if (rt=="0"){
                    state=MENU;
                }

                // check if a user is assigned to the project (5pts)
               // for (auto project: projects) {
               //     for (auto user: project->assignees) {
               //         if (current_user->username == user.second) {
               //             // enter the following options
               //         }
               //     }

                    //INCOMPLETE: view different types of issues & details (5pts + 3pts)
//                if (in == "1") {
//                    for (auto project: current_user->projs) {
//                        for (auto todo_issue: project->todo) {
//                            cout << todo_issue->ID << " " << todo_issue->desc << endl;
//                        }
//                    }
//                }
//
//                else if (in == "2") {
//                    for (auto project: current_user->projs) {
//                        for (auto sprint: project->in_progress) {
//                            for (auto issue: sprint->issues) {
//                                cout << issue->ID << " " << issue->desc << endl;
//                            }
//                        }
//                    }
//                }
//
//                else if (in == "3") {
//
//                        for (auto done_issue: project->done) {
//                            cout << done_issue->ID << " " << done_issue->desc << endl;
//                        }
//                    }
//                }




                    break;
                }
                case CREATE_PROJ: {
                    //create a project and edit it, assign people, fundamental project stuff
                    vector<Issue *> done;
                    vector<Sprint *> in_progress;
                    vector<Issue *> todo;
                    //user pointer and string of permissions
                    vector<pair<User *, string>> assignees;

                    string name;
                    string due;
                    string desc;

                    cout << "Project name: " << endl;
                    getline(cin, name);

                    cout << "Project due date: " << endl;
                    getline(cin, due);

                    cout << "Description: " << endl;
                    getline(cin, desc);

                    pair<User*,string> t = {current_user, "own"};
                    //push into a list of to_do issues
                    projects.push_back(new Project(name, due, desc));
                    projects[projects.size()-1]->assignees.push_back(t);
                    current_user->projs.push_back(projects[projects.size()-1]);
                    cout<<"Project created. If you wish to assign members or issues, do it from 'edit projects'\n";
                    state=MENU;

                    break;
                }

                //create issue (5pts)
                case CREATE_ISSUE: {
                    string projname;
                    string desc;
                    string type;
                    string prio;
                    for(auto p: current_user->projs){cout<<p->name<<endl;}
                    cout << "Please enter project name from the list above (exactly as typed): " << endl;
                    getline(cin, projname);
                    Project* x = 0;
                    for(auto p: current_user->projs){if(projname==p->name){x=p;break;}}
                    cout << "Please enter issue type: " << endl;
                    getline(cin, type);

                    cout << "Please enter issue description: " << endl;
                    getline(cin, desc);
                    cout << "Please enter issue priority (todo, done): " << endl;
                    getline(cin, prio);
                    count++;
                    if(prio=="done"){
                        x->done.push_back(new Issue(count, desc, type,  prio));
                        x->done[x->done.size() - 1]->proj = x;
                        bool pcheck = false;
                        for(auto s: x->assignees){
                            if(s.first->username==current_user->username){
                                if(s.second=="led" or s.second=="own"){pcheck=true;}
                                break;
                            }
                        }
                        if(pcheck) {
                            cout << "assign a user? y/n\n";
                            string resp;
                            getline(cin, resp);
                            if (resp == "y") {
                                string us;
                                cout << "enter a username that already exists.";
                                getline(cin, us);
                                cout<<"assign to project as mem (member) or led (leader)? enter abbreviated version\n";
                                string psa;
                                cin>>psa;
                                for (auto u: users) {
                                    if (u->username == us) {
                                        x->done[x->done.size() - 1]->assignees.push_back(u);
                                        x->assignees.push_back({u,psa});
                                        u->projs.push_back(x);
                                        break;
                                    }
                                }
                            }
                        }else{
                            x->done[x->done.size() - 1]->assignees.push_back(current_user);
                        }
                    }else{
                        x->todo.push_back(new Issue(count, desc, type,  prio));
                        x->todo[x->todo.size() - 1]->proj = x;
                        bool pcheck = false;
                        for(auto s: x->assignees){
                            if(s.first->username==current_user->username){
                                if(s.second=="led" or s.second=="own"){pcheck=true;}
                                break;
                            }
                        }
                        if(pcheck) {
                            cout << "assign a user? y/n\n";
                            string resp;
                            getline(cin, resp);
                            if (resp == "y") {
                                string us;
                                cout << "enter a username that already exists.\n";
                                getline(cin, us);
                                cout<<"assign to project as mem (member) or led (leader)? enter abbreviated version\n";
                                string psa;
                                cin>>psa;
                                for (auto u: users) {
                                    if (u->username == us) {
                                        x->todo[x->todo.size() - 1]->assignees.push_back(u);
                                        x->assignees.push_back({u,psa});
                                        u->projs.push_back(x);
                                        break;
                                    }
                                }
                            }
                        }else{
                            x->todo[x->todo.size() - 1]->assignees.push_back(current_user);
                        }
                    }
                    clear_cli();
                    cout<<"Done. Returning to projects.\n";
                    state=PROJS;

                    break;
                }
                case CREATE_SPRINT: {
                    for(auto p: current_user->projs){cout<<p->name<<endl;}
                    cout << "Please enter project name from the list above (exactly as typed): " << endl;
                    string projname;
                    getline(cin, projname);
                    Project* x = 0;
                    for(auto p: current_user->projs){if(projname==p->name){x=p;break;}}
                    //add issues from todos to sprints
                    int timeframe;
                    cout<<"enter a number for timeframe 1 or 2 weeks\n";
                    cin >> timeframe;
                    int tcount = 0;
                    sprints.push_back(new Sprint());
                    sprints[sprints.size()-1]->timeframe = timeframe;
                    while (true) {
                        for (auto is: x->todo) {
                            cout << "index:" << tcount << endl;
                            tcount++;
                            displayis(is);
                        }
                        cout<<"chose the index of the issue you want to add to the sprint\n";
                        int ind;
                        cin>>ind;
                        if(ind<x->todo.size()) {
                            sprints[sprints.size() - 1]->issues.push_back(x->todo[ind]);
                            x->todo.erase(x->todo.begin()+ind);
                            if(x->todo.empty()){break;}
                            cout << "type 'n' to stop adding issues.\n";
                            string dec;
                            cin >> dec;
                            if (dec == "n") { break;x->in_progress.push_back(sprints[sprints.size()-1]); }
                        }
                    }
                    state=PROJS;
                    clear_cli();
                    cout<<"Sprint created\n";
                    break;
                }

                case PROJS: {
                    //access projects assigned to you
                    //depending on perms, edit certain things (check rubric and user perspective paper)

                    //always access through current user

                    if (!current_user->projs.empty()) {
                        for (auto project: current_user->projs) {
                            cout << "Name: "<<project->name << "\nDue: " << project->due <<"\nDesc: "<<project->desc<< endl;
                            string perms;
                            for (auto m: project->assignees){
                                if((m.first)->username==current_user->username){
                                    perms = m.second;
                                    break;
                                }
                            }
                            //lead/owner can view everything in a project
                            if(perms=="led" or perms=="own"){
                                cout<<"To do..\n";
                                for(auto td: project->todo){
                                    displayis(td);
                                }
                                cout<<"\nSprint..\n";
                                for(auto sp: project->in_progress){
                                    for(auto sis: sp->issues){
                                        displayis(sis);
                                    }
                                }
                                cout<<"\nDone...\n";
                                for(auto dn: project->done){
                                    displayis(dn);
                                }
                                //edit things about the project
                                cout<<"Enter the number corresponding to intended action.\n";
                                cout<<"[1]: Create new issue for a project\n[2]: Create a sprint for a project\n";
                                cout<<"[3]: Edit project\n[4]: Assign Users to a Project\n[5]: Assign Users to an issue\n";
                                cout<<"[6]: Return to menu\n";
                                string in;
                                getline(cin, in);
                                if (in == "1") {


                                    state=CREATE_ISSUE;
                                } else if (in == "2") {


                                    state=CREATE_SPRINT;
                                }else if (in == "3") {

                                    for(auto p: current_user->projs){cout<<p->name<<endl;}
                                    cout << "Please enter project name from the list above (exactly as typed): " << endl;
                                    string projname;
                                    getline(cin, projname);
                                    Project* x = 0;
                                    for(auto p: current_user->projs){if(projname==p->name){x=p;break;}}
                                    string name;
                                    string due;
                                    string desc;
                                    cout << "Project name: " << endl;
                                    getline(cin, name);

                                    cout << "Project due date: " << endl;
                                    getline(cin, due);

                                    cout << "Description: " << endl;
                                    getline(cin, desc);
                                    x->name=name;
                                    x->due=due;
                                    x->desc=desc;
                                    clear_cli();
                                    cout<<"project edited.";
                                }else if (in == "5") {
                                    for(auto p: current_user->projs){cout<<p->name<<endl;}
                                    cout << "Please enter project name from the list above (exactly as typed): " << endl;
                                    string projname;
                                    getline(cin, projname);
                                    Project* x = 0;
                                    for(auto p: current_user->projs){if(projname==p->name){x=p;break;}}

                                    string us;
                                    cout << "enter a username that already exists.\n";
                                    getline(cin, us);
                                    cout<<"are they already a member of the project? y/n\n";
                                    string r;
                                    cin >> r;
                                    bool yeah=false;
                                    if(r=="n"){yeah=true;}
                                    string psa;
                                    if(yeah){
                                    cout<<"assign to project as mem (member) or led (leader)? enter abbreviated version\n";

                                    cin>>psa;}
                                    for (auto u: users) {
                                        if (u->username == us) {
                                            x->todo[x->todo.size() - 1]->assignees.push_back(u);
                                            if(yeah){
                                            x->assignees.push_back({u,psa});
                                            u->projs.push_back(x);}
                                            break;
                                        }
                                    }



                                }else if (in == "4") {
                                    for(auto p: current_user->projs){cout<<p->name<<endl;}
                                    cout << "Please enter project name from the list above (exactly as typed): " << endl;
                                    string projname;
                                    getline(cin, projname);
                                    Project* x = 0;
                                    for(auto p: current_user->projs){if(projname==p->name){x=p;break;}}

                                    string us;
                                    cout << "enter a username that already exists.\n";
                                    getline(cin, us);
                                    cout<<"assign to project as mem (member) or led (leader)? enter abbreviated version\n";
                                    string psa;
                                    cin>>psa;
                                    for (auto u: users) {
                                        if (u->username == us) {
                                            x->assignees.push_back({u,psa});
                                            u->projs.push_back(x);
                                            break;
                                        }
                                    }


                                }else if (in == "6") {
                                    clear_cli();
                                    state=MENU;
                                }

                            }
                            //members can only see their assigned issues
                            else if(perms=="mem"){
                                cout<<"To do..\n";
                                for(auto td: project->todo){
                                    bool ok = false;
                                    for(auto per: td->assignees){
                                        if(per->username==current_user->username){
                                            ok = true;
                                            break;
                                        }
                                    }
                                    if(ok){displayis(td);}

                                }
                                cout<<"\nSprint..\n";
                                for(auto sp: project->in_progress){
                                    for(auto sis: sp->issues){
                                        bool ok = false;
                                        for(auto per: sis->assignees){
                                            if(per->username==current_user->username){
                                                ok = true;
                                                break;
                                            }
                                        }
                                        if(ok){displayis(sis);}
                                    }
                                }
                                cout<<"\nDone\n";
                                for(auto dn: project->done){
                                    bool ok = false;
                                    for(auto per: dn->assignees){
                                        if(per->username==current_user->username){
                                            ok = true;
                                            break;
                                        }
                                    }
                                    if(ok){displayis(dn);}
                                }

                                //members can only create issues
                                cout<<"Enter the number corresponding to intended action.\n";
                                cout<<"[1]: Create new issue for a project\n[2]: Return to menu\n";
                                string in;
                                getline(cin, in);
                                if (in == "1") {
                                    state=CREATE_ISSUE;
                                } else if (in == "2") {
                                    state=MENU;
                                }
                            }

                        }
                    }else{
                        cout<<"You have no projects.\n[0]: Return to menu\n";
                        string b;
                        cin>>b;
                        if(b=="0"){state=MENU;}
                    }



/*
                    cout << "Enter the number corresponding to intended action." << endl;
                    cout << "[1]: Change project name\n[2]: Change project Due date \n[3]: Change project Description\n"
                         << "[4]: View project description" << endl
                         << "[9]: Main menu"
                         << endl;
*/
                    break;
                }
                case EXIT: {
                    cout << "Exiting...\n";
                    //handle dealloc
                    if (!users.empty()) {
                        for (int i = 0; i < users.size(); i++) {
                            delete users[i];
                        }
                    }
                    if (!issues.empty()) {
                        for (int i = 0; i < issues.size(); i++) {
                            delete issues[i];
                        }
                    }
                    if (!sprints.empty()) {
                        for (int i = 0; i < sprints.size(); i++) {
                            delete sprints[i];
                        }
                    }
                    if (!projects.empty()) {
                        for (int i = 0; i < projects.size(); i++) {
                            delete projects[i];
                        }
                    }
                    exit = true;
                    break;
                }
            }
        }
    }

