::First I used qaq to clean up registry a
qaq

::Then I used qa to record a new macro
qa

::To search for 'Name', I used /Name
/name

::To delete characters between current cursor position and U, I used d/U
d/U

::Join the next line by using shift+J (i.e. uppercase J)
J

::To delete characters between current cursor position and :(included), I used d/: and then x to delete one character
d/:
x

::Join the next line and delete to : again
J
d/:
x

::Stop recording using q
q


::After recording a macro, I first search how many 'Name' remaining using :%s/Name/&/gn
:%s/Name/&/gn

::Knowing that there are still 3499 'Name', I run the macro 3499 more times
3499@a

