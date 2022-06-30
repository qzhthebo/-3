using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace AppTest_V2._0
{
    public partial class RemoteControl : UserControl
    {
        float[] x = new float[16];
        public RemoteControl()
        {
            InitializeComponent();
        }
        private void RemoteControl_Load(object sender, EventArgs e)
        {

        }
        public void chuanzhi(float[] b)
        {

           for(int i=0;i<9;i++)
            {
                x[i] = b[i];
            }
                progressBar1.Value = (int)x[0];
                label17.Text = x[0].ToString();
                progressBar2.Value = (int)x[1];
                label18.Text = x[1].ToString();
                progressBar3.Value = (int)x[2];
                label19.Text = x[2].ToString();
                progressBar4.Value = (int)x[3];
                label20.Text = x[3].ToString();
                progressBar5.Value = (int)x[4];
                label21.Text = x[4].ToString();
                progressBar6.Value = (int)x[5];
                label22.Text = x[5].ToString();
                progressBar7.Value = (int)x[6];
                label23.Text = x[6].ToString();
                progressBar8.Value = (int)x[7];
                label24.Text = x[7].ToString();
                progressBar9.Value = (int)x[8];
                label25.Text = x[8].ToString();
                progressBar10.Value = (int)x[9];
                label26.Text = x[9].ToString();
                progressBar11.Value = (int)x[10];
                label27.Text = x[10].ToString();
                progressBar12.Value = (int)x[11];
                label28.Text = x[11].ToString();
                progressBar13.Value = (int)x[12];
                label29.Text = x[12].ToString();
                progressBar14.Value = (int)x[13];
                label30.Text = x[13].ToString();
                progressBar15.Value = (int)x[14];
                label31.Text = x[14].ToString();
                progressBar16.Value = (int)x[15];
                label32.Text = x[15].ToString();
            

        }

        private void button1_Click(object sender, EventArgs e)
        {
           try
            {
                SaveFileDialog savefiledialog = new SaveFileDialog();//实例化打开文件控件
                savefiledialog.Filter = "文本文件|*.txt";//可保存的文件格式
                if (savefiledialog.ShowDialog() == DialogResult.OK)
                {
                    string str;
                    string FileName = DateTime.Now.Year.ToString() + "/" + DateTime.Now.Month.ToString() + "/" +
                                      DateTime.Now.Day.ToString() + ":" + DateTime.Now.Second.ToString();
                    StreamWriter streamwriter = new StreamWriter(savefiledialog.FileName);//实例化保存文件控件

                    str = x[0].ToString() + " " + x[1].ToString() + " " + x[2].ToString() + " "
                        + x[3].ToString() + "  " + x[4].ToString() + "  " + x[5].ToString() +
                        "  " + x[6].ToString() + " " + x[7].ToString() + " " + x[8].ToString()
                        + "  " + x[9].ToString() + "  " + x[10].ToString() + "  " +
                        x[11].ToString() + "  " + x[12].ToString() + "  " + x[13].ToString() +
                        " " + x[14].ToString() + " " + x[15].ToString() + "  \n ";
                    streamwriter.Write(str);
                    streamwriter.Flush();//刷新缓存
                    streamwriter.Close();//关闭流

                }
            }
            catch
            {
                Console.WriteLine("报错：保存文件失败！");
            }
        }
    }
}
