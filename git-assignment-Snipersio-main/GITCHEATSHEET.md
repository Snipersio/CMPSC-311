# `git` Cheat Sheet

The following is a useful list of commands that you will most likely frequently use as a programmer using `git` 
VCS (GitHub, GitLab, etc...). For this course, we will use the GitHub service as it is the most ubiquitous and 
free to use.

There is a misconception that `git` and GitHub are the same thing. It is important to distinguish them: `git` is an 
open source VCS developed by Linux creator Linus Torvalds, while GitHub provides a cloud storage service for `git`-based
repositories, currently owned by Microsoft. You do not need GitHub to use `git`, and the commands below are valid for 
any `git`-based service (GitLab, private server, industry, etc...)



# `git` Terminology

Here is a dictionary of common terms used by `git` and shared among other VCS's:
 - **repository** - (or repo) A collection of source/project files
 - **remote/origin** - A cloud/server source of the repo (think root or ground truth)
 - **local** - A local/personal copy of the repo
 - **branch** - A version of the repo with a distinct collection of changes
 - **main/master** - The typical name for the initial/root/main/ground truth branch
 - **head** - The current state in a repo/branch
 - **diff** - The difference between two states of an item; such as a file (changing contents, creating/moving/renaming/removing file)
 - **not staged** - A diff that has not been saved/checkpointed yet
 - **staged** - A diff that has been checkpointed
 - **commit** - A collection of one or more staged diffs to apply to a repo/branch
 - **push** - The act of committing/applying a commit to the remote repo/branch
 - **pull** - The act of acquiring new remote commits to your local repo/branch
 - **merge** - The act of combining commits/branches into one commit/branch



# `git` Installation

(only done once)

Git is already installed on the CSE machines for you. However, you might want to download `git` on your personal 
machine, so you can use a visual IDE instead of a terminal-based one -- your choice. If you choose to install `git` 
locally, you can install it [here](https://git-scm.com/downloads).



# `git` Setup

(only done once, usually)

Once `git` is installed you will want to configure / tell `git` who you are. You can use the following commands to
set your information:
 - To set/change your name:
   - `git config --global user.name “firstname lastname”`
 - To set/change your email:
   - `git config --global user.email “abc1234@psu.edu”`

Why this is useful: If you install `git` via another service, such as GitHub, these will be automatically set for you 
at login. However sometimes it may be useful to change what name you have based on the machine you are working on; 
Doing this per-machine gives you that option should you find necessary.



# `git` Project Initialization

There are two ways to initialize a project; by creating or cloning. 
 - If you are making a new project, navigate into a directory that will serve as the root directory of the project and run:
   - `git init`
   - (for this course you won't be using this as GitHub will create the repository for you. However, in other classes where GitHub might not be used, but you want to use git to track your project progress, this is when this would be used.)
 - If you are cloning an existing project from a remote source (such as GitHub, GitLab, another git server...) you'll run:
   - `git clone <destination>`
   - There are two ways to clone; via HTTPS or via SSH. 
     - If you clone via HTTPS then the web URL (beginning with `https://` and ending in `.git`) is all you need.
     - If you clone via SSH then you will need to address the `.git` location with your credentials. For example:
       - `git clone abc1234@domain.com:<path>.git`
   - (GitHub makes this easy for you; click the green "Code" dropdown and select HTTPS or SSH, then copy to clipboard)
   

# `git` Basics

These are common commands you will use (some frequently) throughout your development. They are organized somewhat in a linear / logical way. 

To check the local state of your repository:
 - `git status`
   - This tells you what branch you are on (master, main, ...)
   - What changes are not staged (usually in red)
   - What changes are staged (usually in green)
   - How many local commits are uncommitted/not pushed
   - What files are in merge conflict

To stage a diff into a new commit:
 - `git add [file/directory]`
   - After making your changes use this command to create a new commit. 
   - You can stage a single file or multiple at a time.
     - For example, `git add test.c` stages just one file
     - Or `git add test.c src/test.h` stages two files.
   - You can also stage directories:
     - For example, `git add .` stages all files in the current directory
     - Or, `git add src/` stages all files in the `src` directory
   - You can also use globs:
     - For example, `git add src/*.h` stages all files in the `src` directory that have file names ending in `.h` 
 - You can call `git add` multiple times for a new commit, it does not have to be all at once
 - If you stage a file, then edit that file, the **stage diff remains the same**. If you want to include the newer changes then stage the file again.
 
To unstage a diff from a new commit:
 - `git reset [file/directory]`
   - If you stage changes but later do not want to commit them, unstage the diffs with this command. 
   - Like `git add`, you can call this multiple times on multiple files, directories, and globs.

To revert unstaged files back to their original state:
  - `git restore [file/directory]`
    - `git restore` was added ~2019. The original way to do this was `git checkout -- [file/directory]` instead. This command still works today.
  - This will remove any changes made and restore it back to the head state.

To commit a collection of staged files:
  - `git commit -m "a description of your changes"`
    - To create a commit of your staged changes, use this command. 
    - Be sure to give a thorough description of your changes. (relevant [xkcd](https://xkcd.com/1296/))
  - Note: this only commits to your local repo!

To undo a local commit (that has not been pushed):
  - If you make commits but no longer want to commit them to the origin, you have two options;
  - If you want to keep the changes the commit made and put them in an unstaged state, use:
    - `git reset HEAD~1`
    - This will pop 1 commit from the head. If you want to undo 2 non-pushed commits use HEAD~2 and so on...
  - If you want to discard the local changes:
    - `git reset --hard HEAD~1`
    - Warning: this will also discard any other local uncommitted changes (staged or unstaged!)
    - ***Warning: this is destructive and prior changes are unrecoverable. Be careful.***

To push local changes/commits to the remote/origin repo:
  - `git push`
    - This will take any uncommitted commits you have locally and push them to the origin. 
    - You can only push when your earliest new commit is after the origins head.
      - You will get an error that the remote has changes you don't have otherwise.
      - To fix this use `git pull`, then `git push` again provided no conflicts.

To pull remote commits and make sure your local repo/branch is up-to-date:
 - `git pull`
   - If you get "Already up to date", then your local branch is at the remote branches head (good)
   - If your latest local commit is before all new remote commits then all later commits will be downloaded and merged in (good)
   - If your latest local commit is after any of the new remote commits then you will have a **merge conflict**. (bad) See later section below.

To undo a pushed commit:
  - `git revert [commit ID/hash]`
    - This will take all diffs from the specified commit and undo them. 
    - Creates a new commit (effectively uncommitting the specified commit) which will then need to be pushed/pulled

To view the history of commits/changes for a repository:
  - `git log`
    - This will give you a list of all commits in chronological order. 
    - Each entry is an overview of that commit containing:
      - When it was committed, who it was (name \<email\>),the commit message, and the commit ID/hash

Sometimes it's useful to see changes between unstaged files, commits, etc...
  - diffs are shown as colors and symbols.
    - red text (-) are deletions
    - green text (+) are additions
    - white text is unchanged
  - When viewing a diff, use the up/down arrow keys or page up / down keys to navigate. Type `q` when you'd like to exit the diff view
  - `git diff`
    - View all current unstanged changes
  - `git diff --staged`
    - View all current staged (but not committed) changes
  - `git diff [file]`
    - View unstaged changes of a single file
  - `git diff [commit ID/hash]`
    - View the difference between your local repo's head and the specified commit. (Can get IDs from the log)
    - This will show you all changes between these two points in time
  - `git diff [branch]`
    - View the difference between your local repo's head and the specified branch's head
  
To get a list of all branches in a repository:
  - `git branch`
    - Your current branch will have a `*` next to it
    - Or alternatively you can check with `git status`
  - If you do not see branches in this list that you know exist, try `git pull` first

To create a new branch from the current branch's head:
  - `git branch [branch name]`
    - on older versions of git the command was `git checkout -b [branch name]`. This method still works too. 
  - Your local uncommitted changes will follow you into the new branch

To change to an existing branch:
  - `git checkout [branch name]`
    - This will change the current branch to the specified branch and update all local files that branch's head.
    - Warning: You cannot change branches with uncommitted changes

To merge two branches:
  - `git merge [branch name]`
    - This will merge all commits in `[branch name]` ***into*** the current branch. 
    - Just like pulling, if the latest commit to a file is before all new branch commits to that file then the merge is done (good)
    - If any commit to a file is after any of the new branch commits then you will have a merge conflict. (bad) See later section below.

... and many more. Feel free to look at the `git` documentation to learn more about the commands and their modifiers/arguments!

# `git` Merge Conflicts

Yikes, you found yourself in a merge conflict. Not to worry though, these can* be easy to solve 
(*relevant [xkcd](https://xkcd.com/1597/)). Whether you got here 
via a `git merge` or a `git pull`, both are handled the same. In essence, you modified a file that was modified by 
someone else before you received that update (meaning, you both had the same starting point, but your commits put you 
in different ending points). This means you will have to decide whether to keep your (local) changes or the 
other (remote) changes. Let's look at an example. Suppose you have a file that looks originally like this:

```
This is a rambling statement.
I like to write words.
Words words words...
```

Someone committed the following (remote):

```
This is a rambling statement.
I like loooooooooooooong words.
Words words words...
```

And you have the following committed (local):

```
This is a rambling statement.
This is a statement.
Words words words...
```

This is a merge conflict. When you attempt to merge, `git` will lazily add both versions into the conflicting file(s)
and tell you which files have conflicts (via `git status`). At this point you should open each merge conflict file to address the issue.
Opening our example file, you will see something like this:

```
This is a rambling statement.
<<<<<<< HEAD
I like loooooooooooooong words.
=======
This is a statement.
>>>>>>> local
Words words words...
```

Here we find the conflicting lines of the remote head and our local commit, seperated by `<<<<<<<`, `=======`, 
and `>>>>>>>` delimiters. To resolve the conflict, simply delete the delimiters and the lines you don't want / keep
the lines you want to keep. Let's say we want to keep our (local) changes. In the preceding example we would delete 
the lines:

```
<<<<<<< HEAD
I like loooooooooooooong words.
=======
>>>>>>> local
```
 
leaving the file to look like:

```
This is a rambling statement.
This is a statement.
Words words words...
```

We can then commit this merge back to the remote/origin to resolve the conflict. Rinse and repeat for all other merge files.
