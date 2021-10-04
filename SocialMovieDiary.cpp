
#include<bits/stdc++.h>
#include <fstream>
#include<string>
using namespace std;

class Movie
{
    private:
        int year;
        int critic_score;
        int viewCount;
        int aud_score;
        string title;
        string director;
        string genres;
    public:

        int getYear()
        {
            return this->year;
        }
        int getCriticScore()
        {
            return this->critic_score;
        }
        int getViewCount()
        {
            return this->viewCount;
        }
        int getAudienceScore()
        {
            return this->aud_score;
        }
        string getTitle()
        {
            return this->title;
        }
        string getDirector()
        {
            return this->director;
        }
        string getGenres()
        {
            return this->genres;
        }
        void setYear(int y)
        {
            this->year=y;
        }
        void setCriticScore(int cs)
        {
            this->critic_score=cs;
        }
        void setViewCount(int vc)
        {
            this->viewCount=vc;
        }
        void setAudienceScore(int as)
        {
            this->aud_score=as;
        }
        void setTitle(string t)
        {
            this->title=t;
        }
        void setDirector(string d)
        {
            this->director=d;
        }
        void setGenres(string g)
        {
            this->genres=g;
        }
        void updateViewCount()
        {
            this->viewCount++;
        }
        void updateAudScore(int s)
        {
            this->aud_score=(this->aud_score*(this->viewCount-1)+s)/this->viewCount;
        }
        void printDetails()
        {
            cout<<"Title: "<<title<<endl;
            cout<<"Director: "<<director<<endl;
            cout<<"Genre: "<<genres<<endl;
            cout<<"Release Year: "<<year<<endl;
            cout<<"No. of views: "<<viewCount<<endl;
            cout<<"Critic Score: "<<critic_score<<endl;
            cout<<"Audience Score: "<<aud_score<<endl;
        }
        string getCsvForm()
        {
            return director+","+genres+","+title+","+to_string(year)+","+to_string(critic_score)+","+to_string(viewCount)+","+to_string(aud_score)+"\n";
        }
};

class BTNode
{
    Movie *keys;
    int minDegree;
    BTNode **children;
    int currentKeys;
    bool isLeaf;
public:
    BTNode(int minDegree , bool isLeaf)
    {
        this->minDegree=minDegree;
        this->isLeaf=isLeaf;

        keys= new Movie[2*minDegree-1];
        children= new BTNode *[2*minDegree];

        currentKeys=0;
    }

    void traverse()
    {
        int i;
        for (i = 0; i < currentKeys; i++)
        {
            if (!isLeaf)
                children[i]->traverse();
            keys[i].printDetails();
        }
        if (!isLeaf)
            children[i]->traverse();
    }

    void traverseByRating(int r)
    {
        int i;
        for (i = 0; i < currentKeys; i++)
        {
            if (!isLeaf)
                children[i]->traverseByRating(r);
            if(keys[i].getCriticScore()>r)
                keys[i].printDetails();
        }
        if (!isLeaf)
            children[i]->traverseByRating(r);
    }
    void writeMovies(ofstream& fptr)
    {
        int i;
        for (i = 0; i < currentKeys; i++)
        {
            if (!isLeaf)
                children[i]->writeMovies(fptr);
            string temp=keys[i].getCsvForm();
            fptr<<temp;
        }
        if (!isLeaf)
            children[i]->writeMovies(fptr);
    }

    int search(string query)
    {
        int i = 0;
        while (i < currentKeys && keys[i].getTitle().compare(query)<0)
        {
            i++;
            cout<<keys[i].getTitle()<<endl;
        }

        if (keys[i].getTitle().compare(query) == 0)
        {
            keys[i].printDetails();
            return 1;
        }
        if (!isLeaf)
            return children[i]->search(query);
        else
        {
            cout<<"\nNo movie found\n";
            return 0;
        }
    }

    void updateMovieData(string query,int rating)
    {
        int i = 0;
        while (i < currentKeys && keys[i].getTitle().compare(query)<0)
            i++;

        if (keys[i].getTitle().compare(query) == 0)
        {
            keys[i].updateViewCount();
            keys[i].updateAudScore(rating);
            return;
        }
        if (!isLeaf)
            return children[i]->updateMovieData(query,rating);\
        else{
            cout<<"\nNo movie found\n";
        return;
        }
    }

    void insertNonFull(Movie m)
    {
        int i = currentKeys-1;
        if (isLeaf)
        {
            while (i >= 0 && keys[i].getTitle().compare(m.getTitle())>0)
            {
                keys[i+1] = keys[i];
                i--;
            }
            keys[i+1] = m;
            currentKeys++;
        }
        else
        {
            while (i >= 0 && keys[i].getTitle().compare(m.getTitle())>0)
                i--;
            if (children[i+1]->currentKeys == 2*minDegree-1)
            {
                splitChild(i+1, children[i+1]);
                if (keys[i+1].getTitle().compare(m.getTitle()) < 0)
                    i++;
            }
            children[i+1]->insertNonFull(m);
        }
    }

    void splitChild(int i, BTNode *left)
    {
        BTNode *right = new BTNode(left->minDegree, left->isLeaf);
        right->currentKeys = minDegree - 1;
        for (int j = 0; j < minDegree-1; j++)
            right->keys[j] = left->keys[j+minDegree];

        if (!left->isLeaf)
        {
            for (int j = 0; j < minDegree; j++)
                right->children[j] = left->children[j+minDegree];
        }

        left->currentKeys = minDegree - 1;
        for (int j = currentKeys; j >= i+1; j--)
            children[j+1] = children[j];

        children[i+1] = right;

        for (int j = currentKeys-1; j >= i; j--)
            keys[j+1] = keys[j];

        keys[i] = left->keys[minDegree-1];
        currentKeys = currentKeys + 1;
    }

    friend class BT;
};

class BT
{
    BTNode *root;
    int minDegree;
    public:

    BT(int minDegree)
    {
        root = NULL;
        this->minDegree = minDegree;
    }

    void traverse()
    {
        if(root != NULL)
            root->traverse();
    }
    void traverseByRating(int r)
    {
        if(root!=NULL)
            root->traverseByRating(r);
    }

    int search(string query)
    {
        if(root == NULL)
        {
            cout<<"\nEMPTY TREE\n";
            return 0;
        }
        return root->search(query);
    }

    void update(string query,int rating)
    {

        return root->updateMovieData(query,rating);
    }

    void insert(Movie m)
    {
        if (root == NULL)
        {
            root = new BTNode(minDegree,true);
            root->keys[0] = m;
            root->currentKeys = 1;
        }
        else
        {
            if (root->currentKeys == 2*minDegree-1)
            {
                BTNode *newNode = new BTNode(minDegree, false);
                newNode->children[0] = root;
                newNode->splitChild(0, root);
                int i = 0;
                if (newNode->keys[0].getTitle().compare(m.getTitle()) < 0)
                    i++;
                newNode->children[i]->insertNonFull(m);
                root = newNode;
            }
            else
                root->insertNonFull(m);
        }
    }

    void readFromFile()
    {
        ifstream file("movieData.csv");
        if(!file.is_open())
            cout << "ERROR: File Open" << '\n';
        string director,genres,title,year,criticScore,viewCount,audScore;
        Movie temp;
        while(file.good()){
            getline(file,director,',');
            getline(file,genres,',');
            getline(file,title,',');
            getline(file,year,',');
            getline(file,criticScore,',');
            getline(file,viewCount,',');
            getline(file,audScore,'\n');
            temp.setDirector(director);
            temp.setGenres(genres);
            temp.setTitle(title);
            temp.setYear(stoi(year));
            temp.setCriticScore(stoi(criticScore));
            temp.setViewCount(stoi(viewCount));
            temp.setAudienceScore(stoi(audScore));
            insert(temp);
        }
        file.close();
    }

    void outputFile()
    {
        ofstream file("movieData.csv");
        if(!file.is_open())
            cout << "ERROR: File Open" << '\n';
        root->writeMovies(file);
        file.close();
    }
};

class Person
{
    public:
    string username;
    unordered_set<string> watched;
    unordered_set<string> watchlist;
    Person(string n)
    {
        username=n;
    }
    void addtoWatched(string movieName,BT obj)
    {
        if(obj.search(movieName))
        {
            int r;
            watched.insert(movieName);
            cout<<"\nMovie Inserted to Watched";
            cout<<"\nHow would you rate the movie?";
            cin>>r;
            cout<<endl;
            obj.update(movieName,r);
        }else
        {
            cout<<"\nMovie Not Found\n";
        }
    }
    void addtoWatchList(string movieName,BT obj)
    {
        if(obj.search(movieName))
        {
            watchlist.insert(movieName);
        }else
        {
            cout<<"\nMovie Not Found\n";
        }

    }
    void showWatched()
    {
        cout<<"\nUser: "<<this->username<<endl;
        for(auto &j: this->watched)
                cout<<j<<endl;
        cout<<endl<<endl;
    }

    void showWatchList()
    {
        cout<<"\n\tUser: "<<this->username<<endl;
        for(auto &j: this->watchlist)
            cout<<j<<endl;
        cout<<endl<<endl;
    }

};

class SocialNetwork
{
    public:
        unordered_map<string,unordered_set<string>> followers,following;
        unordered_map<string,Person*> users;

    int registerUser(string username)
    {
        if(users.find(username)==users.end())
        {
            users[username] = new Person(username);
            return 1;
        }
        else
        {
            cout<<"User already exists\n";
            return 0;
        }
    }

    bool addConnection(string otherUser, string loggedInUser)
    {
        if(followers[otherUser].find(loggedInUser)==followers[otherUser].end())
        {
            followers[otherUser].insert(loggedInUser);
            following[loggedInUser].insert(otherUser);
            cout<<"\nNow User: "<<users[loggedInUser]->username<<" is following User: "<<users[otherUser]->username<<" on social network";
            return true;
        }
        return false;
    }
    Person* login()
    {
        string u;
        cout<<"\nEnter username\n";
        cin>>u;
        if(users.find(u)==users.end())
            cout<<"\nUser Does Not Exist\n";
        else
        {
            cout<<"\nLogin Successful\n";
            return users[u];
        }
    }
    void showFollowers(string a)
    {
        cout<<"\n\tUser: "<<users[a]->username<<" Followers are: \n";
        for(auto &i: followers[a])
            cout<<i<<" ";
        cout<<endl<<endl;
    }

    void showFollowing(string a)
    {
        cout<<"\n\tUser: "<<users[a]->username<<" is Following: \n";
        for(auto &i: following[a])
            cout<<i<<" ";
        cout<<endl<<endl;
    }

    void createNetwork()
    {
        vector<string> usernames={"John","William","James","Charles","George","Frank","Joseph","Thomas","Henry","Robert"};
        for(auto &i:usernames)
            users[i]=new Person(i);
        for(int i=1;i<=10;){
            int a=rand()%usernames.size();
            int b=rand()%usernames.size();
            if(a!=b){
                if(addConnection(usernames[a],usernames[b]))
                    i++;
            }
        }
    }

    bool isFollowing(string user,string followee)
    {
        if(following[user].find(followee)==following[user].end())
            return 0;
        return 1;
    }

    void printFollowingMovie(string user,string followee)
    {
        if(isFollowing(user,followee))
        {
            users[followee]->showWatched();
        }
    }


};

int main()
{
    cout<<"\nProcessing Data\n";
    SocialNetwork network;
    network.createNetwork();
    BT obj(10);
    obj.readFromFile();
    Person *mainUser;
    cout<<"\nWelcome to The Movie Diary Social Network\n";
    int flag=1,isLoggedIn=0;
    int ch1;
    while(flag)
    {
        cout<<"\nPress 1 to register\n";
        cout<<"\nPress 2 to login\n";
        cin>>ch1;
        if(ch1==1)
        {
            string u;
            cout<<"\nEnter Username\n";
            cin>>u;
            network.registerUser(u);



        }else if(ch1==2)
        {
            mainUser=network.login();
            if(mainUser!=NULL)
            {
                flag=0;
                isLoggedIn=1;
            }
        }else
        {
            cout<<"\nInvalid Choice\n";
        }
    }
    int ch2;
    while(isLoggedIn)
    {
        cout<<"\nEnter 1 to add connection\n";
        cout<<"\nEnter 2 to add movie to watched\n";
        cout<<"\nEnter 3 to add movie to watchlist\n";
        cout<<"\nEnter 4 to show watched movies\n";
        cout<<"\nEnter 5 to show watchlist\n";
        cout<<"\nEnter 6 to show who follows you\n";
        cout<<"\nEnter 7 to show who you follow\n";
        cout<<"\nEnter 8 to see all movie list\n";
        cout<<"\nEnter 9 to see all movie list with rating greater than x\n";
        cout<<"\nEnter 10 to see your follower's movies\n";
        cout<<"\nEnter 11 to logout\n";
        cin>>ch2;
        switch(ch2)
        {
            case(1):
                {
                    string u;
                    cout<<"\nEnter Username\n";
                    cin>>u;
                    if(network.addConnection(u,mainUser->username))
                    {
                        cout<<"\nSuccessful\n";
                    }else
                    {
                        cout<<"\nNot Successful\n";
                    }
                    break;
                }
            case(2):
                {
                    string m;
                    cout<<"\nEnter Movie Name\n";
                    cin.clear();
                    cin.sync();
                    getline(cin,m);
                    mainUser->addtoWatched(m,obj);
                }
                break;
            case(3):
                {
                    string m;
                    cout<<"\nEnter Movie Name\n";
                    cin.clear();
                    cin.sync();
                    getline(cin,m);
                    mainUser->addtoWatchList(m,obj);
                    break;
                }
            case(4):
                {
                    mainUser->showWatched();
                    break;
                }
            case(5):
                {
                    mainUser->showWatchList();
                    break;
                }
            case(6):
                {
                    network.showFollowers(mainUser->username);
                    break;
                }
            case(7):
                {
                    network.showFollowing(mainUser->username);
                    break;
                }
            case(8):
                {
                    obj.traverse();
                    break;
                }
            case(9):
                {
                    int r;
                    cout<<"\nEnter rating\n";
                    cin>>r;
                    obj.traverseByRating(r);
                    break;
                }
            case(10):
                {
                    string f;
                    cout<<"\nEnter followee name\n";
                    cin>>f;
                    network.printFollowingMovie(mainUser->username,f);
                    break;
                }

            case(11):
                {
                    mainUser=NULL;
                    obj.outputFile();
                    return 0;
                }
        }
    }
}
