from sewar import uqi
import cv2
import ffmpeg

import os

import img2pdf

import glob

from random import randint

from tkinter import *

from tkinter import filedialog

from PIL import ImageTk, Image

import shutil

import ntpath

#-----------------------------

class App:
    def __init__(self,root_window):
        #size of window
        
        self.root = root_window

        self.width = 800
        self.heigth = 700

        self.folder_name = 'aa'

        self.file = 'none'

        self.slide_list= []

        self.w_org = 0
        self.h_org = 0

        self.w_res=0 
        self.h_res = 0

        self.crop_position = [10,10,100,100]

        self.mode_toogle = True

        #windows size, icon and title
        self.root.geometry(f"{self.width}x{self.heigth}")
        self.root.iconbitmap("icon.ico")
        self.root.title("video2pdf")
        self.root.resizable(False,False)



        self.root.bind("<Left>", self.left)
        self.root.bind("<Right>", self.right)
        self.root.bind("<Up>", self.up)
        self.root.bind("<Down>", self.down)
        self.root.bind("<space>", self.mode)


        self.root.update()

        print(f'window:\nwidth:{self.root.winfo_width()} | heigth:{self.root.winfo_height()}\n')


        text_upper_bar = Label(self.root, font=8, text="Przy pomocy strzałek  zaznacz interesujący Cię obszar\n użyj SPACJI aby przełązać sie między trybami ustawienia obszaru \n wybrany obszar to ten zawierający się w prostokącie\n obramówka się nie liczy")
        text_upper_bar.grid(row = 0, column= 0,columnspan=4)

        self.text_mode_selected = Label(self.root, font=10, text="Move mode")
        self.text_mode_selected.grid(row = 1, column= 0,columnspan=4)
        # thumbnail()


        #read thumbnail


        image = Image.open("thumbnail.png")
        self.w_org, self.h_org =image.size
        image = image.resize((int(self.root.winfo_width()),int((self.root.winfo_width())/self.w_org*self.h_org)), Image.ANTIALIAS)
        self.w_res, self.h_res =image.size
        print(f'image orginal:\nwidth:{self.w_org} | heigth:{self.h_org}\n')
        print(f'image resize:\nwidth:{self.w_res} | heigth:{self.h_res}\n')
        image = ImageTk.PhotoImage(image)

        img2 = Image.open("thumbnail.png")

        # img2 = ImageTk.PhotoImage(img2)

        self.canvas = Canvas(self.root, bg='grey', width=self.root.winfo_width(), height=int(self.root.winfo_width()/self.w_res*self.h_res) )

        self.canvas.grid(row = 4, column= 0, columnspan=4)

        print(f'canvas:\nwidth:{self.canvas.winfo_width()} | heigth:{self.canvas.winfo_height()}\n')

        self.image_container = self.canvas.create_image(self.root.winfo_width()/2,int(self.root.winfo_width()/self.w_res*self.h_res)/2, image=image, anchor=CENTER)

        self.rect = self.canvas.create_rectangle(196,144,610,436, outline = "green", width=6, fill="")

        button = Button(self.canvas, text="Button", width=10,state="disabled")




        ## make a button in canvas to choose the folder
        # button.place(x=width/2, y=300,anchor=CENTER)
        # button.place(x=100, y=10,anchor=CENTER)
        # button.config(command=lambda: click_action(button1,"button"))
        # button.place_forget()



        text1 = Label(self.root, font=10, text="Wygeneruj podgląd",pady=10)
        text1.grid(row = 6, column= 0)

        text2 = Label(self.root, font=10, text="Rozpocznij ekstrakcję")
        text2.grid(row = 6, column= 1)

        text3 = Label(self.root, font=10, text="Usuń podobne slajdy")
        text3.grid(row = 6, column= 2)

        text4 = Label(self.root, font=10, text="Wygeneruj PDF")
        text4.grid(row = 6, column= 3)



        button1 = Button(self.root, text="Click", width=10)
        button1.config(command=lambda: self.click_action(button1,"choose_file_action"))
        button1.grid(row = 7, column= 0, pady=10)


        button2 = Button(self.root, text="Click", width=10)
        button2.config(command=lambda: self.click_action(button2,"start_extraction_action"))
        button2.grid(row = 7, column= 1, pady=10)


        button3 = Button(self.root, text="Click", width=10)
        button3.config(command=lambda: self.click_action(button3,"delete_similar_action"))
        button3.grid(row = 7, column= 2, pady=10)


        button4 = Button(self.root, text="Click", width=10)
        button4.config(command=lambda: self.click_action(button4,"to_pdf_action"))
        button4.grid(row = 7, column= 3, pady=10)


        self.text_position = Label(self.root, font=10, text=f"Canva position: {self.canvas.coords(self.rect)}",pady=10)
        self.text_position.grid(row = 8, column= 0,columnspan = 2)
        self.text_position_true = Label(self.root, font=10, text=f"Crop position: {self.crop_position}",pady=10)
        self.text_position_true.grid(row = 8, column= 1, columnspan= 3)


    def wideo_to_image(self,file_name = "none",x1=458,y1=344,x2=1478,y2=1067,export_fps = 120):

        # os.mkdir("./"+file)
        # file_name = self.file
        folder= os.path.splitext(file_name)[0]
        os.mkdir(folder)
        
        stream = ffmpeg.input(file_name)
        stream = ffmpeg.filter(stream,'fps', fps='1/120')
        #stream = ffmpeg.filter(stream,'strftime')
        stream = ffmpeg.crop(stream,x1,y1,x2-x1,y2-y1)
        stream = ffmpeg.output(stream,folder+'/test-%d.png',start_number=1)
        #stream = ffmpeg.output(stream,folder+'/%Y-%m-%d_%H-%M-%S_test.png')
        stream = ffmpeg.overwrite_output(stream)
        ffmpeg.run(stream)


    def wideo_to_image_decimate(file = "none",x1=458,y1=344,x2=1478,y2=1067,export_fps = 120):

        # os.mkdir("./"+file)
        folder= os.path.splitext(file)[0]
        os.mkdir(folder)
        
        stream = ffmpeg.input(file)
        stream = ffmpeg.filter(stream,'decimate', scthresh='10')
        
        #stream = ffmpeg.filter(stream,'strftime')
        stream = ffmpeg.crop(stream,x1,y1,x2-x1,y2-y1)
        stream = ffmpeg.output(stream,folder+'/test-%d.png',start_number=1)
        #stream = ffmpeg.output(stream,folder+'/%Y-%m-%d_%H-%M-%S_test.png')
        stream = ffmpeg.overwrite_output(stream)
        ffmpeg.run(stream)


    def image_del_duplicate(self):


        folder_name= os.path.splitext(self.file)[0]

        files = glob.glob("./"+folder_name+"/*.png")   
        files.sort(key=os.path.getmtime)
        print(files)
        print("____________")

        for i in range(len(files)):
            files[i] = files[i].replace("./"+folder_name+"\\","") 

        print (files)

        print("____________")

        # for i, item in enumerate(files):
        #     files[i] = item.replace("./"+folder_name+"\\","") 

        # print (files)



        for i in range(len(files)):

            try:


                img1 = cv2.imread("./"+folder_name +"/"+ files[i],1)

                img2 =cv2.imread("./"+folder_name +"/"+ files[i+1],1)

                similarity = uqi(img1,img2,8)

                print(files[i] + " | " + files[i+1] + "| similarity: " + str(similarity))

                if(similarity<0.99):
                    self.slide_list.append(files[i+1])



            except:
                print("end of array")


        print(self.slide_list)

        for i in range(len(self.slide_list)):
            self.slide_list[i] = "./" + folder_name+"/" + self.slide_list[i]

        print(self.slide_list)

        folder= os.path.splitext(self.file)[0]
        os.mkdir(folder + "_extracted")

        for i in range(len(self.slide_list)):
            shutil.copy(self.slide_list[i], "./" + folder_name+"_extracted")

        


    def images_directory_to_pdf(self):

        folder_name= os.path.splitext(self.file)[0]

        files = glob.glob("./"+folder_name+"_extracted/*.png")   
        files.sort(key=os.path.getmtime)
        print(files)
        print("____________")

        for i in range(len(files)):
            files[i] = files[i].replace("./"+folder_name+"_extracted\\","") 

        print (files)

        for i in range(len(files)):
            files[i] = "./" + folder_name+"_extracted/" + files[i]

        print(files)

        with open(f"{self.file}.pdf","wb") as f:
            f.write(img2pdf.convert(files))


    def thumbnail(self,file):

        stream = ffmpeg.input(file)
        # stream = ffmpeg.filter(stream,'fps', '1/30')

        stream = ffmpeg.filter(stream,'select','eq(n,'+str(3*6*randint(400, 1200))+')')

        stream = ffmpeg.output(stream,'thumbnail.png',vframes=1)
        # stream = ffmpeg.output(stream,'test/test-%d.png',start_number=1)
        stream = ffmpeg.overwrite_output(stream)
        ffmpeg.run(stream)





    #function for gui
    def click_action(self,button,action):
        if(action == "thumbnail_action"):
            pass

        elif(action == "choose_file_action"):
            filename = filedialog.askopenfilename(initialdir="./", title="Wybierz plik video")
            

            self.file = ntpath.basename(f"r{filename}")

            print(self.file)
            self.thumbnail(self.file)

            # global img2
            self.img2 = Image.open("thumbnail.png")
            self.img2 = self.img2.resize((int(self.root.winfo_width()),int((self.root.winfo_width())/self.w_org*self.h_org)), Image.ANTIALIAS)
            self.img2 = ImageTk.PhotoImage(self.img2)
            self.canvas.itemconfig(self.image_container,image = self.img2)


        elif(action == "start_extraction_action"):
            print(action)
            button.config(text=f"Generowanie ...",state=DISABLED)
            self.wideo_to_image(self.file,self.crop_position[0],self.crop_position[1],self.crop_position[2],self.crop_position[3] )
            button.config(text=f"Wygenerój",state=ACTIVE)
        
        elif(action == "delete_similar_action"):
            print(action)
            button.config(text=f"Generowanie ...",state=DISABLED)
            self.image_del_duplicate()
            button.config(text=f"Click",state=ACTIVE)
        
        elif(action == "to_pdf_action"):
            print(action)
            self.images_directory_to_pdf()

        else:
            print(action)



    def position_staff(self):
        x0, y0, x1, y1 = self.canvas.coords(self.rect)
        self.crop_position[0] = int(x0 * self.w_org/self.w_res)
        self.crop_position[1] = int(y0 * self.w_org/self.w_res)
        self.crop_position[2] = int(x1 * self.w_org/self.w_res)
        self.crop_position[3] = int(y1 * self.w_org/self.w_res)
        self.text_position.configure(text=f"Canva position: {self.canvas.coords(self.rect)}")
        self.text_position_true.configure(text=f"Crop position: {self.crop_position}")
        print(f"Canva: {self.canvas.coords(self.rect)}")
        print(f"Crop{self.crop_position}")


    def right(self,eveny):
        if (self.mode_toogle):
            x = 2
            y = 0
            self.canvas.move(self.rect,x,y)
        else:
            x0, y0, x1, y1 = self.canvas.coords(self.rect) # get the coords of rect
            x1 = x1 + 2                        # calc new coords
            self.canvas.coords(self.rect, x0, y0, x1, y1)  # set new coords

        self.position_staff()

    def left(self,eveny):
        if (self.mode_toogle):
            x = -2
            y = 0
            self.canvas.move(self.rect,x,y)

        else:
            x0, y0, x1, y1 = self.canvas.coords(self.rect) # get the coords of rect
            x1 = x1 - 2                        # calc new coords
            self.canvas.coords(self.rect, x0, y0, x1, y1)  # set new coords
        self.position_staff()

    def up(self,eveny):
        if (self.mode_toogle):
            x = 0
            y = -2
            self.canvas.move(self.rect,x,y)

        else:
            x0, y0, x1, y1 = self.canvas.coords(self.rect) # get the coords of rect
            y1 = y1 -2                        # calc new coords
            self.canvas.coords(self.rect, x0, y0, x1, y1)  # set new coords
        self.position_staff()

    def down(self,eveny):
        if (self.mode_toogle):
            x = 0
            y = 2
            self.canvas.move(self.rect,x,y)

        else:
            x0, y0, x1, y1 = self.canvas.coords(self.rect) # get the coords of rect
            y1 = y1 + 2                        # calc new coords
            self.canvas.coords(self.rect, x0, y0, x1, y1)  # set new coords
        self.position_staff()

    def mode(self,eveny):

        # global mode_toogle
        
        self.mode_toogle  = (not self.mode_toogle)

        if(self.mode_toogle):
            print("Move mode")
            self.text_mode_selected.configure(text="Move mode")

        else:
            print("Scale mode")
            self.text_mode_selected.configure(text="Scale mode")


def main(): 
    rooted = Tk()
    app = App(rooted)
    rooted.mainloop()


if __name__ == '__main__':
    main()