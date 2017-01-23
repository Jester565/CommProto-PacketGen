# PacketGenerator for CommProtocol Library
##Please read PacketGen_Guide.docx for much more information

##Description
Converts simple message files to C++ or C# code for packets.

##Using the precompiled version
1. Clone the repository
2. Go into the directory with the name of your OS
3. Run the executable <b>PacketGen</b> with these option<br/>
 -I is the path to your input file (required)<br/>
 -O is the path to the directory you want your output files in (defaults to current directory)<br/>
 -L is the language (defaults to cpp)<br/>
 -L=cpp is c++<br/>
 -L=cs  is c#<br/>
 -E is the file extension (defaults to .h on cpp and .cs on c#)<br/>

Example:
 PacketGen -I=../../example/NGCP_Templates.ngen -O=./generated/cpp/ -L=cpp -E=hpp

##Building the project
1. Clone the repository
2. Create a build folder in the folder for the repository
3. Run this command in the build folder: cmake ../
4. You can now open the solution if you are using visual studio or call 'make' if on linux
5. Visual studio users will have to enter in command line arguments for the program to work properly. To do this, right click the PacketGen project in solution explorer, select properties, select Debugging, Edit Command Arguments to what you want
