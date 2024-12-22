#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
using namespace std;

const int MAX_USERS = 50;
const int MAX_POSTS = 100;
const int MAX_GROUPS = 10;
const int MAX_FRIENDS = 10;
const int MAX_GROUP_MEMBERS = 10;

class Post {
public:
    string content;
    int likes;
    string comments[10];
    int commentCount;

    Post() : likes(0), commentCount(0) {}

    void createPost(string text) {
        content = text;
        likes = 0;
        commentCount = 0;
    }

    void likePost() {
        likes++;
    }

    void addComment(string comment) {
        if (commentCount < 10) {
            comments[commentCount++] = comment;
        } else {
            cout << "Maximum comments reached!\n";
        }
    }

    void display() {
        cout << "Post: " << content << "\nLikes: " << likes << "\nComments:\n";
        for (int i = 0; i < commentCount; i++) {
            cout << "- " << comments[i] << "\n";
        }
    }
};

class User;

class Group {
public:
    string groupName;
    User* owner;
    User* members[MAX_GROUP_MEMBERS];
    int memberCount;
    Post groupPosts[MAX_POSTS];
    int groupPostCount;

    Group() : memberCount(0), groupPostCount(0), owner(NULL) {}

    void createGroup(string name, User* creator) {
        groupName = name;
        owner = creator;
        addUser(creator); // Add creator as the first member
    }

    void addUser(User* user) {
        if (memberCount < MAX_GROUP_MEMBERS) {
            members[memberCount++] = user;
        } else {
            cout << "Maximum group members reached!\n";
        }
    }

    void addGroupPost(string content) {
        if (groupPostCount < MAX_POSTS) {
            groupPosts[groupPostCount++].createPost(content);
        } else {
            cout << "Maximum group posts reached!\n";
        }
    }

    void viewGroupPosts() {
        cout << "Group: " << groupName << "\n";
        for (int i = 0; i < groupPostCount; i++) {
            cout << "Post " << i + 1 << ":\n";
            groupPosts[i].display();
            cout << "\n";
        }
    }
};

class User {
protected:
    string username;
    string password;
    string name;
    int age;
    string friendList[MAX_FRIENDS];
    int friendCount;
    Group* groups[MAX_GROUPS];
    int groupCount;
    Post posts[MAX_POSTS];
    int postCount;

public:
    User() : friendCount(0), groupCount(0), postCount(0) {}

    void signUp(string uname, string pwd, string unameFull, int userAge) {
        username = uname;
        password = pwd;
        name = unameFull;
        age = userAge;
        cout << "User " << username << " signed up successfully.\n";
    }

    bool login(string uname, string pwd) {
        if (username == uname && password == pwd) {
            cout << "Login successful!\n";
            return true;
        }
        cout << "Invalid username or password.\n";
        return false;
    }

    string getUsername() const {
        return username;
    }

    void addFriend(string friendName, User users[], int userCount) {
        if (friendCount < MAX_FRIENDS) {
            bool found = false;
            for (int i = 0; i < userCount; i++) {
                if (users[i].getUsername() == friendName) {
                    friendList[friendCount++] = friendName;
                    cout << "Friend added successfully.\n";
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "User not found!\n";
            }
        } else {
            cout << "Maximum friends reached!\n";
        }
    }

    void createPost(string content) {
        if (postCount < MAX_POSTS) {
            posts[postCount++].createPost(content);
        } else {
            cout << "Maximum posts reached!\n";
        }
    }

    void viewPosts() {
        for (int i = 0; i < postCount; i++) {
            cout << "Post " << i + 1 << ":\n";
            posts[i].display();
            cout << "\n";
        }
    }

    void joinGroup(Group* group) {
        if (groupCount < MAX_GROUPS) {
            groups[groupCount++] = group;
            group->addUser(this);
            cout << "Joined group " << group->groupName << " successfully.\n";
        } else {
            cout << "You are already part of the maximum number of groups.\n";
        }
    }

    void viewProfile() {
        cout << "\n--- Your Profile ---\n";
        cout << "Username: " << username << "\n";
        cout << "Full Name: " << name << "\n";
        cout << "Age: " << age << "\n";
        cout << "Posts:\n";
        for (int i = 0; i < postCount; i++) {
            posts[i].display();
            cout << "\n";
        }
        cout << "Groups:\n";
        for (int i = 0; i < groupCount; i++) {
            cout << groups[i]->groupName << "\n";
        }
    }

    void downloadProfileInfo() {
        ofstream file("detail.txt");
        if (!file.is_open()) {
            cout << "Error opening file to write profile information.\n";
            return;
        }

        file << "Name: " << name << "\n";
        file << "Age: " << age << "\n";
        file << "Username: " << username << "\n";
        file << "Posts:\n";
        for (int i = 0; i < postCount; i++) {
            file << posts[i].content << "\n";  // Save the content of each post
        }
        file << "Groups:\n";
        for (int i = 0; i < groupCount; i++) {
            file << groups[i]->groupName << "\n";  // Save the name of each group the user belongs to
        }
        file.close();
        cout << "Profile information saved to file: detail.txt\n";
    }

    void viewFriendProfile(User* friendUser) {
        // Show friend's basic info
        cout << "\n--- Friend Profile ---\n";
        cout << "Username: " << friendUser->getUsername() << "\n";
        cout << "Full Name: " << friendUser->name << "\n";
        cout << "Age: " << friendUser->age << "\n";

        // Show friend's posts
        cout << "\nFriend's Posts:\n";
        for (int i = 0; i < friendUser->postCount; i++) {
            cout << "Post " << i + 1 << ": " << friendUser->posts[i].content << "\n";
        }

        // Show friend's groups
        cout << "\nFriend's Groups:\n";
        for (int i = 0; i < friendUser->groupCount; i++) {
            cout << "Group " << i + 1 << ": " << friendUser->groups[i]->groupName << "\n";
        }

        // Provide options to like, join, or comment
        int action;
        do {
            cout << "\nWhat would you like to do?\n";
            cout << "1. Like a Post\n2. Join a Friend's Group\n3. Comment on a Post\n4. Go Back\n";
            cout << "Enter your choice: ";
            cin >> action;
			system("cls");
            switch (action) {
                case 1: {
                    int postIndex;
                    cout << "Enter the post number you want to like (1 to " << friendUser->postCount << "): ";
                    cin >> postIndex;
                    if (postIndex >= 1 && postIndex <= friendUser->postCount) {
                        friendUser->posts[postIndex - 1].likePost();
                        cout << "Liked post " << postIndex << ".\n";
                    } else {
                        cout << "Invalid post number.\n";
                    }
                    break;
                }
                case 2: {
                    int groupIndex;
                    cout << "Enter the group number you want to join (1 to " << friendUser->groupCount << "): ";
                    cin >> groupIndex;
                    if (groupIndex >= 1 && groupIndex <= friendUser->groupCount) {
                        joinGroup(friendUser->groups[groupIndex - 1]);
                        cout << "Joined group " << friendUser->groups[groupIndex - 1]->groupName << ".\n";
                    } else {
                        cout << "Invalid group number.\n";
                    }
                    break;
                }
                case 3: {
                    int postIndex;
                    cout << "Enter the post number you want to comment on (1 to " << friendUser->postCount << "): ";
                    cin >> postIndex;
                    if (postIndex >= 1 && postIndex <= friendUser->postCount) {
                        string comment;
                        cout << "Enter your comment: ";
                        cin.ignore();  // Clear the input buffer
                        getline(cin, comment);
                        friendUser->posts[postIndex - 1].addComment(comment);
                        cout << "Comment added to post " << postIndex << ".\n";
                    } else {
                        cout << "Invalid post number.\n";
                    }
                    break;
                }
                case 4:
                    cout << "Going back...\n";
                    break;
                default:
                    cout << "Invalid choice. Try again.\n";
            }
        } while (action != 4);
    }
};

// Main Menu
void userMenu(User* currentUser, User users[], int userCount, Group groups[], int& groupCount) {
    int choice;
    do {
        cout << "\n--- User Menu ---\n";
        cout << "1. Create Post\n2. View Posts\n3. Add Friend\n4. Join Group\n5. View Profile\n6. Download Profile Information\n7. View Friend Profile\n8. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;
		system("cls");
        switch (choice) {
            case 1: {
                string content;
                cout << "Enter post content: ";
                cin.ignore();
                getline(cin, content);
                currentUser->createPost(content);
                break;
            }
            case 2:
                currentUser->viewPosts();
                break;
            case 3: {
                string friendName;
                cout << "Enter friend's username: ";
                cin.ignore();
                getline(cin, friendName);
                currentUser->addFriend(friendName, users, userCount);
                break;
            }
            case 4: {
                if (groupCount < MAX_GROUPS) {
                    string groupName;
                    cout << "Enter group name: ";
                    cin.ignore();
                    getline(cin, groupName);
                    groups[groupCount].createGroup(groupName, currentUser);
                    currentUser->joinGroup(&groups[groupCount]);
                    groupCount++;
                } else {
                    cout << "Maximum groups reached!\n";
                }
                break;
            }
            case 5:
                currentUser->viewProfile();
                break;
            case 6:
                currentUser->downloadProfileInfo();
                break;
            case 7: {
                string friendName;
                cout << "Enter friend's username: ";
                cin.ignore();
                getline(cin, friendName);
                for (int i = 0; i < userCount; i++) {
                    if (users[i].getUsername() == friendName) {
                        currentUser->viewFriendProfile(&users[i]);
                        break;
                    }
                }
                break;
            }
            case 8:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 8);
}

int main() {
    User users[MAX_USERS];
    Group groups[MAX_GROUPS];
    int userCount = 0;
    int groupCount = 0;
    User* currentUser = NULL;  

    int option;
    do {
        cout << "\n--- Main Menu ---\n";
        cout << "1. Sign Up\n2. Login\n3. Exit\n";
        cout << "Enter your choice: ";
        cin >> option;
		system("cls");
        switch (option) {
            case 1: {
                string uname, pwd, unameFull;
                int userAge;
                cout << "Enter username: ";
                cin >> uname;
                cout << "Enter password: ";
                cin >> pwd;
                cout << "Enter full name: ";
                cin.ignore();
                getline(cin, unameFull);
                cout << "Enter age: ";
                cin >> userAge;
                users[userCount].signUp(uname, pwd, unameFull, userAge);
                userCount++;
                break;
            }
            case 2: {
                string uname, pwd;
                cout << "Enter username: ";
                cin >> uname;
                cout << "Enter password: ";
                cin >> pwd;
                bool loggedIn = false;
                for (int i = 0; i < userCount; i++) {
                    if (users[i].login(uname, pwd)) {
                        currentUser = &users[i];
                        userMenu(currentUser, users, userCount, groups, groupCount);
                        loggedIn = true;
                        break;
                    }
                }
                if (!loggedIn) {
                    cout << "Login failed!\n";
                }
                break;
            }
            case 3:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (option != 3);

    return 0;
}

