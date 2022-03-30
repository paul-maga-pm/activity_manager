# description
A small C++ Qt desktop application for managing hobby activities. For each activity, informations about name, description, type and durations are stored. The application
can perform CRUD operations on the activities list and undo after updating the list of activities. Activity manager uses an in-memory wishlist which offers CRUD operations, generating a wishlist from existing activities and saving the wishlist in a csv file.

# GUI
![Screenshot 2022-03-30 171136](https://user-images.githubusercontent.com/93886764/160855273-f60a4691-24b3-4e47-b4c1-21ab874e0d8e.png)

# Console interface
![Screenshot 2022-03-30 171250](https://user-images.githubusercontent.com/93886764/160855525-647428da-5e23-41ad-8136-291068fbd2ea.png)


# implementation details
The application uses Qt framework for GUI component. The architecture respects the layered architecture model: Domain (activity model and validators), Persistence (activity repositories), Business (services and whislist services) and Presentation (GUI or console). Also, the application can be used with a console interface which
implements some of the features of the GUI one. The services use a repository interface (abstract class), so the type of persistence (in-memory or csv-file) can be
changed without affecting the code in the business layer.

The implementation respects the guidlines of modern C++ (C++ 17).

# prerequisites
  - Visual Studio IDE
  - Qt 6.2.3 library for msvc2019_64
  - C++ Qt extension for Visual Studio
