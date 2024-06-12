using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace TFT_Bitmap_Editor
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            progressBar1.Value = 0;
            if(openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                Bitmap image1;
                string dir = openFileDialog1.FileName;
                string fileName = openFileDialog1.SafeFileName;
                pictureBox1.ImageLocation = dir;
                image1 = new Bitmap(dir);
                textBox1.Text = "File Name: " + fileName + "\r\n";
                textBox1.Text += "Width " + Convert.ToString(image1.Width) + "\r\n";
                textBox1.Text += "Height " + Convert.ToString(image1.Height);
                button2.Enabled = true;
            }
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            Bitmap image1 = new Bitmap(pictureBox1.Image);
            string path = @"c:\MyTest.txt";
            string hex;
            byte r, g, b;
            int i, j, k, red, green, blue, color16Bit;
            if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                string fileName = Path.GetFileNameWithoutExtension(openFileDialog1.SafeFileName);
                path = folderBrowserDialog1.SelectedPath + @"\" + "Bitmap " + fileName + ".txt";
                FileInfo fi = new FileInfo(path);
                if (!fi.Exists)
                {
                    using (StreamWriter sw = File.CreateText(path))
                    {                        
                        sw.WriteLine("const unsigned int " + fileName + "[] = {");
                        k = 0;
                        for (i = 0; i < image1.Height; i++)
                        {
                            for (j = 0; j < image1.Width; j++)
                            {
                                Color pixelColor = image1.GetPixel(j, i);
                                r = pixelColor.R;
                                g = pixelColor.G;
                                b = pixelColor.B;
                                red = r; green = g; blue = b; color16Bit = 0;
                                red = (red & 248) << 8;
                                green = (green & 252) << 3;
                                blue = (blue & 248) >> 3;
                                color16Bit = (red | green | blue);                                                               
                                if ((i + 1 == image1.Height) && (j + 1 == image1.Width))
                                {
                                    hex = "0x" + color16Bit.ToString("X4");
                                }
                                else
                                {
                                    hex = "0x" + color16Bit.ToString("X4") + "," + " ";
                                }
                                if (k > 9)
                                {
                                    k = 0;
                                    sw.WriteLine();
                                }
                                k += 1;
                                sw.Write(hex);                               
                            }                            
                        }
                        sw.WriteLine();
                        sw.WriteLine("};");
                        progressBar1.Value = 100;
                    }
                }
            }
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
