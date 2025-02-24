# General Guidelines Team 02 - Mobility Matrix

## How to create a new issue
When creating a new issue, the naming of the issue should define clearly the main topic, or related story and epic, followed by a short description of the task.
Before all the fields required on a new issue are filled in, issues are maintained with the status “Backlog Review”. 
After defining all required properties, the status should be changed to “Backlog Ready”.

### Creating an epic

If an epic is being created, a short name or description defining the project or feature should be used for title.
While creating the issue, “epic” label should be added to the issue so that it is easily identifiable.
In the description, under the title Description, a short summary of the main objectives should be added. This description should allow any reader to easily understand what is the main goal of the epic, why it is important to implement.
The stories related to the epic should also be linked in the description of the epic. 
Example:

```
Description
(...)
TASKS
[ ] <link to task1>   
[ ] <link to task2>
(...)
```

A Priority for the epic should be established.

### Creating a story
If a story is being created, the title of the issue should be concise and understandable.
While creating the issue, “story” label should be added to the issue so that it is easily identifiable.
In the description, the first line should be used to state to which epic it is related with a link to that epic. For example:

```
EPIC 
[ ] < link to related epic >
```

After that, a full description of the main goals of the story should be added, under the title Description,  followed by the links of the tasks to which it relates to.

```
Description
(...)
TASKS
[ ] <link to task1>   
[ ] <link to task2>
(...)
```
   
A Priority for the story should be established.
### Creating a task
If a task is being created, the title of the issue should be as long as needed to be fully understandable.
While creating the issue, “task” label should be added to the issue so that it is easily identifiable.
In the description, the first line should be used to state to which story it is related with a link to that story. For example:

```
STORY 
[ ] < link to related story >
```

After that, a full description of the main goals of the task should be added after the title Description. The task can and should be more specific, as small as possible, and define the issue to be solved in a more technical way.
Example:

```
Description
(...)
```

A Priority for the task should be established.
## How to assign and solve an issue/task

In the Project Team02, select the “Current Sprint” tab and select one of the tasks assigned to you from the list of “Todo” and drag it to the “In Progress” column.
If there are no tasks assigned to you and there are tasks left on “Todo”, select one and assign it to you.
Then, click on the task and under the “Development” window on the right side of the page, select  “Create a branch” to create a new branch to solve the issue.

### Rules for the branch naming
The name of the branch should be on the following format:

`<Issue or bug> - <team_member> - <number>`

### How to create a pull request
On github, click on `Make pull/review request`.
On the window opened, change the main branch on the top left to match the origin repository and add a detailed description with all changes made to the code.  



## General organisation rules

### Placing new tasks on backlog
Tasks, stories and epics can be added at any time to the “Backlog Review” column of the project. Adding any new issue will automatically set it in “Backlog Review”.
Descriptions and final detailing of the issues can be added at any time also, when finished, the issue should be moved to”Backlog Ready”.  

### Retrospective objectives and meeting frequency
At a starting stage of the development, the sprints will take 1 week so that the project development can be more closely monitored.
Internal (within the group) retrospectives will be held every week, in which meeting we will also prepare the todo list for the following week.
A retrospective with the all SEA:ME teams will be held every other week. 

### File indexing in repositories
	When creating a new repository (module), the file structure should follow the pattern:

```
Project folder
|_____src
|_____include
|_____tests
|_____(...)
```







