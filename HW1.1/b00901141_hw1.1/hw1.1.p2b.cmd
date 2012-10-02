::command:

grep -nv "   Darr (nothing)" hw1.1.p2.log | grep "Output Transition Time" | sort -t ":" -k 3 > hw1.1.p2b.log

::explanation:

::'grep' all lines that contain "   Darr (nothing)" pattern with line number prefix and reverse the result, so that the out put will be all lines that do not contain the pattern. Pass the result through a pipe and again, 'grep' all lines containing "Output Tansition Time". Pass the result to 'sort' command. Since a line will now looks like:

::10587:   Output Transition Time: 9.81773

::,thus separate a line with ':' and sort by column 3 will match the requirement.

