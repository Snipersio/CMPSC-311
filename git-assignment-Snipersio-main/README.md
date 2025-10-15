# Assignment Instructions

Follow the steps below carefully. Each step builds upon the previous one, so complete them in order.

---
## Part 1: Setting Up `git` and GitHub
Before working on this assignment, you need to make sure `git` is installed and configured properly. This should be a one-time task for rest of the course.
If you have already used GitHub in another course which used the CSE machines, you can skip this section, otherwise follow it carefully.
Below, we will guide you through generating and configuring SSH keys so you can securely connect to GitHub services and work on assignments via the CSE machines.
Similar steps can be made if you wish to install `git` on your local machine, though it is not required.

---
### 1. Signing Up

To use GitHub, you first need to create an account. To do that, go to https://github.com, click on the Sign-Up button, and proceed to the registration form. Choose a proper username and remember your login credentials.
NOTE: If you already have a GitHub account, you can skip this step. However, you cannot change your GitHub username throughout the semester.

### 2. Installing `git`

Git is already installed on the CSE machines for you. You can verify this by running:

     $ git --help 

However, you might want to download `git` on your personal machine for other projects or for 
moving code back and forth between your CSE machine and your local machine - your choice. 
If you choose to install `git` locally, you can follow the installation guide [here](https://git-scm.com/downloads).

### 3. Configuring `git`

To configure your `git` client (either your CSE machine or your personal machine), set your name and email using the following commands:

    $ git config --global user.name "Your Name"
    $ git config --global user.email "Your Email"

### 4. Authenticating `git` (Adding an SSH key)

GitHub will host your code behind an SSH server for you to connect and pull from. Before you can access this server though, 
you must tell GitHub to trust your computer / connection. To authenticate your client, we will create and add an SSH key to your GitHub account (this of this like a password). 
To do so, follow the below steps: 

#### 4.1 Check for existing SSH keys
Before creating a new SSH key, check if you already have one.

    ls -la ~/.ssh

Look for files named:

- id_rsa.pub, id_ed25519.pub, or similar (.pub means it’s the public key).

If you already have a key you want to use, continue to Step 5 and skip the remaining steps here.



#### 4.2 Generate a new SSH key
Run the following command to create a new SSH key for your machine (replace your email with the one linked to your GitHub account):

    ssh-keygen -t rsa -b 4096 -C "your_email@example.com"

When prompted, press [Enter] to accept the default options (should be 3 times).

When you create the ssh key pair your output will contain a line similar to: `Your public key has been saved in:` 
The directory should be in the ~./ssh directory with file extension ".pub". 

The .pub (public) file is the one you will want to supply to GitHub and use in later steps. Typically, this file is called `id_rsa.pub`
  
---

### 5. Add the SSH key to your GitHub account
Use the `cat` command to dump the .pub file to your terminal. If in the previous step your file was called `id_rsa.pub`, then you would execute:

    cat ~/.ssh/id_rsa.pub

Once the file is printed, we will copy the contents. 
In order to copy the key, select the complete key using your mouse, and after selection, 
left-click to copy the selected text 
(If you are on Mac you may be able to just Cmd+C, Windows 11 users may still be able to Ctrl+C; regardless, left-clicking should work for everyone).

The key should begin with `ssh-rsa` and end with your supplied email.

Once you copied the key text, then:

1. Go to https://github.com/settings/keys
2. Click "New SSH Key".
3. Paste the key into the Key field.
4. Give it a descriptive title (perhaps `CSE Machine`).
5. Click "Add SSH key".

---
Refer to following link for troubleshooting guide
https://docs.github.com/en/authentication/connecting-to-github-with-ssh

You can repeat the above steps for your local machine if you'd like, but again, not necessary.

## ✅ And done!

## Part 2: Accepting the Assignment
For all future projects this is where you will start. That is to say, you will not need to set up `git` anymore. Once the assignment is released, a GitHub invitation link will be posted for you to accept using your GitHub account. 

[Click here to accept this assignment](https://classroom.github.com/a/ZFsPnEFK). Once you accept the invitation, GitHub will create a private repository containing the starter files for the assignment. You will also find an accompanying `README.md`, which is essentially this document you are reading, useful for instructions and further details. In this repository specifically there is also a `GITCHEATSHEET.md` file for you to look through at your own pace / for your future reference. Use as desired.  

## Part 3: The Actual Assignment

### 1. Clone the repository  
   
Navigate to your repository on the GitHub website. You will find a green `Code` or `Clone` button. Click it and copy the **SSH** URL (do not use the HTTPS one).

Navigate back to your CSE machine and execute: 

    $ git clone <repo_ssh_url>

to download your starting repository. The command will download the repository to your present working directory. To see the present working directory, you can use the `pwd` command.
If cloned successfully, you should have a new directory with the name of the repository in your current working directory. You can use the `ls` command to find it.
      
After cloning the repository, move into the repository directory using 

    cd <repo_name>

---

### 2. Create a new text file

Use an editor of your choice (nano, vim, emacs) and create a new text file with name `name.txt`. You can do this by running any **one** of these commands:

    $ nano name.txt
    $ vim name.txt
    $ emacs name.txt

Inside the file write **only your full name** on the first line. Save and exit. You can verify you created and saved the file correctly by using `cat`.  

---

### 3. Add, commit, and push the file
Whenever you change an existing file or add a new file to your local repository, you must stage your change, commit the change, and then push the change to remote repository, i.e. GitHub.
Please refer to the cheatsheet or slides for more info on the following commands / processes.

After creating the text file, you can see the current status of your repository by executing:

    $ git status

Here you should see a red untracked file (the one you just created).

Let's stage this file (`name.txt`) by using the command:

    $ git add name.txt

Now when you check the current status of your repository by executing:

    $ git status

You should see a green "staged" file, ready to be committed. 

Now let's commit this staged change and include a message about what the change was with the command:

    $ git commit -m "Added name.txt"

When we now check the status of the repository using:

    $ git status

You should see no unstaged or staged files, and should see "your branch is ahead by 1 commit."

Finally, push the commit to the remote repository (GitHub) by executing:

    $ git push

Note: you might get an error or warning about "setting an upstream origin". This warning should give you a command to run, which once you do, you can re-run `git push` and all should be sorted.

Congratulations, you just pushed your first change on GitHub! 👏👏 Let's keep going...

---

### 4. Modify the `ID.txt` file  
    
Using an editor of your choice, replace the placeholder text in the `ID.txt` file with your actual details.

Just like before, stage your new change, commit it, then push it with the following commands.

    $ git add ID.txt
    $ git commit -m "Update ID.txt with PSU ID"
    $ git push
    
Feel free to use `git status` as often as you need. 

NOTE: Any time you make changes to *ID.txt*, you must repeat these steps.

---

### 5. Running `script.sh`  

For this assignment you should be using the CSE machines. To prove that you are, run the following command:

    $ ./script.sh

If you see an error like "./script.sh: Permission denied.", add execution permissions using the following command:

    $ chmod +x script.sh

Then run the script again.

After running the script, use `git status` to see what has changed. You will find a new file called `proof`.  Add, commit and push the `proof` file.

### 6. Grading

We have included a grading script that you can use to see what your score will be. Run the grading script as follows:

    $ ./grade.py

Any errors should be addressable / fixable.

---



### 7. Commit IDs

In Git, each commit has a unique identifier called a "commit ID" (or SHA-1 hash). This ID helps you reference specific commits (useful for other commands such as `git diff` and others...). 
To get a history of your commits and their commit IDs, you can use the following command:

    $ git log

The top of the list is the *latest*. You should see your 3 commits (proof, ID.txt, name.txt commits) in the log. You can use the arrow keys to move up and down the list. When you are done type `q`.

If instead of looking at the log you want to just get the latest commit ID, you can use this nifty command:

    $ git log -1 --pretty=format:"%H"

To explain what this is doing:
- `git log`: Shows the commit history.
- `-1`: Limits the output to only the most recent commit.
- `--pretty=format:"%H"`: Displays the full commit hash only.

Importantly, a commit can be thought of as a snapshot of a point in time. Even though a commit ID targets a specific change, all previous changes will be "bundled" with that commit ID.

### 8. Final Deliverable / Submission

In this class you will be submitting a single Commit ID for us to grade you on.
This commit ID will usually be the latest commit you make.
However, each assignment will let you submit multiple times, meaning you can submit one commit ID, then make some changes and submit another commit ID later. 

***Your latest submission (commit ID) via Canvas will be what we grade, so be sure it is the right one and that it has been pushed!***

At the end of this assignment, your repository should contain:
1. `name.txt` with your name.
2. `ID.txt` containing **your actual details**.
3. `proof` file

Make sure all changes are committed and pushed to GitHub. You can verify this by going to the GitHub website, navigating to your repository, and looking at the files and their contents. You can also check the timestamp of the files on the website and see when they were last updated.

Go to Canvas, start the quiz, and submit *just your commit ID* as a single line in the submission box.
