#include "stdafx.h"
#include "Wrapper3D.h"


//  WRAP A NATIVE CLASS

// OTHER EXAMPLE
// native class
/*
class Student
{
private:
char *_fullname;
double _gpa;
public:
Student(char *name, double gpa)
{
_fullname = new char[strlen(name + 1)];
strcpy(_fullname, name);
_gpa = gpa;
}
~Student()
{
delete[] _fullname;
}
double getGpa()
{
return _gpa;
}
char *getName()
{
return _fullname;
}
};


// Managed class

ref class StudentWrapper
{
private:
Student *_stu;
public:
StudentWrapper(String ^fullname, double gpa)
{
_stu = new Student((char *)
System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(
fullname).ToPointer(),
gpa);
}
~StudentWrapper()
{
delete _stu;
_stu = 0;
}

property String ^Name
{
String ^get()
{
return gcnew String(_stu->getName());
}
}
property double Gpa
{
double get()
{
return _stu->getGpa();
}
}
};


*/