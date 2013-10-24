* COURSE ARRANGEMENT

    by Anderain(anderain.develop@gmail.com)
    open source with MPLv2(https://www.mozilla.org/MPL/2.0/)

* HOW TO USE

    Your can try it in command line like this:

    CourseArrange "程设" "计导" "高数" "线代" "文史哲" "思修" -o test.html -i teacher "姜来" -i teacher "毛睿" -i teacher "王志强" -i teacher "张艳" -i teacher "沈琳琳"

    The program while write all possible schedules to test.html.

    -i class <num>|teacher <str>|day <num>
         add a ignore condition it can be class index in a day,teacher's name or day index

    -o
        assign output html file name