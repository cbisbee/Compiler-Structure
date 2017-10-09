import turtle

pen = turtle.Turtle()
pen.hideturtle()
pen.penup()

# Creating the placemarks on the canvas
pen.setx(46.05)
pen.sety(57.08)
pen.write("Mee Canyon",align="left", font=("Arial",13,"normal"))
pen.sety(47.08)
pen.setx(56.05)
pen.write("A canyon in the southwest",align="left", font=("Arial",10,"normal"))
pen.setx(-30.98)
pen.sety(170.89)
pen.write("Knowles Canyon",align="left", font=("Arial",13,"normal"))
pen.sety(160.89)
pen.setx(-20.98)
pen.write("Another canyon in the southwest",align="left", font=("Arial",10,"normal"))
pen.setx(100.9)
pen.sety(300.65)
pen.write("Jones Canyon",align="left", font=("Arial",13,"normal"))
pen.sety(290.65)
pen.setx(110.9)
pen.write("A nearly inaccessible canyon in the southwest near Knowles and Mee Canyon",align="left", font=("Arial",10,"normal"))
pen.setx(-200)
pen.sety(-205.79)
pen.write("Turkey Flats",align="left", font=("Arial",13,"normal"))
pen.sety(-215.79)
pen.setx(-190)
pen.write("An open space resource area",align="left", font=("Arial",10,"normal"))
# End of the placemarks section

turtle.done()
