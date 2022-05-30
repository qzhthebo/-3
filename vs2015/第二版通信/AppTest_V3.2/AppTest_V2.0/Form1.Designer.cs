namespace AppTest_V2._0
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.ILmain = new System.Windows.Forms.ImageList(this.components);
            this.ILremo = new System.Windows.Forms.ImageList(this.components);
            this.ILpara = new System.Windows.Forms.ImageList(this.components);
            this.ILimu = new System.Windows.Forms.ImageList(this.components);
            this.ILgps = new System.Windows.Forms.ImageList(this.components);
            this.RBser = new RichButton.RitchButton();
            this.ILser = new System.Windows.Forms.ImageList(this.components);
            this.MBmain = new MutexButton.MutexButton();
            this.MBimu = new MutexButton.MutexButton();
            this.MBpara = new MutexButton.MutexButton();
            this.MBremo = new MutexButton.MutexButton();
            this.MBgps = new MutexButton.MutexButton();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.tbBaudRate = new System.Windows.Forms.TextBox();
            this.cbComNum = new System.Windows.Forms.ComboBox();
            this.serialPort = new System.IO.Ports.SerialPort(this.components);
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.label3 = new System.Windows.Forms.Label();
            this.pictureBox6 = new System.Windows.Forms.PictureBox();
            this.pictureBox5 = new System.Windows.Forms.PictureBox();
            this.btreno = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.pictureBox4 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.timer2 = new System.Windows.Forms.Timer(this.components);
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox6)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox5)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // ILmain
            // 
            this.ILmain.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ILmain.ImageStream")));
            this.ILmain.TransparentColor = System.Drawing.Color.Transparent;
            this.ILmain.Images.SetKeyName(0, "main_no.jpg");
            this.ILmain.Images.SetKeyName(1, "main_pass.jpg");
            this.ILmain.Images.SetKeyName(2, "main_no.jpg");
            this.ILmain.Images.SetKeyName(3, "main_down.jpg");
            // 
            // ILremo
            // 
            this.ILremo.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ILremo.ImageStream")));
            this.ILremo.TransparentColor = System.Drawing.Color.Transparent;
            this.ILremo.Images.SetKeyName(0, "remo_no.jpg");
            this.ILremo.Images.SetKeyName(1, "remo_pass.jpg");
            this.ILremo.Images.SetKeyName(2, "remo_down.jpg");
            this.ILremo.Images.SetKeyName(3, "remo_down.jpg");
            // 
            // ILpara
            // 
            this.ILpara.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ILpara.ImageStream")));
            this.ILpara.TransparentColor = System.Drawing.Color.Transparent;
            this.ILpara.Images.SetKeyName(0, "para_no.jpg");
            this.ILpara.Images.SetKeyName(1, "para_pass.jpg");
            this.ILpara.Images.SetKeyName(2, "para_down.jpg");
            this.ILpara.Images.SetKeyName(3, "para_down.jpg");
            // 
            // ILimu
            // 
            this.ILimu.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ILimu.ImageStream")));
            this.ILimu.TransparentColor = System.Drawing.Color.Transparent;
            this.ILimu.Images.SetKeyName(0, "imu-no.jpg");
            this.ILimu.Images.SetKeyName(1, "imu-pass.jpg");
            this.ILimu.Images.SetKeyName(2, "imu-down.jpg");
            this.ILimu.Images.SetKeyName(3, "imu-down.jpg");
            // 
            // ILgps
            // 
            this.ILgps.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ILgps.ImageStream")));
            this.ILgps.TransparentColor = System.Drawing.Color.Transparent;
            this.ILgps.Images.SetKeyName(0, "GPS_no.jpg");
            this.ILgps.Images.SetKeyName(1, "GPS-pass.jpg");
            this.ILgps.Images.SetKeyName(2, "GPS-down.jpg");
            this.ILgps.Images.SetKeyName(3, "GPS-down.jpg");
            // 
            // RBser
            // 
            this.RBser.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.RBser.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.RBser.Location = new System.Drawing.Point(666, 224);
            this.RBser.Name = "RBser";
            this.RBser.Size = new System.Drawing.Size(98, 48);
            this.RBser.TabIndex = 12;
            this.RBser.Load += new System.EventHandler(this.RBser_Load);
            this.RBser.Click += new System.EventHandler(this.RBser_Click);
            // 
            // ILser
            // 
            this.ILser.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ILser.ImageStream")));
            this.ILser.TransparentColor = System.Drawing.Color.Transparent;
            this.ILser.Images.SetKeyName(0, "open_no.jpg");
            this.ILser.Images.SetKeyName(1, "open_pass.jpg");
            this.ILser.Images.SetKeyName(2, "open_down.jpg");
            this.ILser.Images.SetKeyName(3, "0.png");
            this.ILser.Images.SetKeyName(4, "close_no.jpg");
            this.ILser.Images.SetKeyName(5, "close_pass.jpg");
            this.ILser.Images.SetKeyName(6, "close_down.jpg");
            this.ILser.Images.SetKeyName(7, "close_pass.jpg");
            // 
            // MBmain
            // 
            this.MBmain.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.MBmain.Location = new System.Drawing.Point(10, 64);
            this.MBmain.Name = "MBmain";
            this.MBmain.Size = new System.Drawing.Size(98, 48);
            this.MBmain.TabIndex = 15;
            this.MBmain.Load += new System.EventHandler(this.MBmain_Load);
            this.MBmain.Click += new System.EventHandler(this.MBmain_Click);
            // 
            // MBimu
            // 
            this.MBimu.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.MBimu.Location = new System.Drawing.Point(10, 227);
            this.MBimu.Name = "MBimu";
            this.MBimu.Size = new System.Drawing.Size(98, 48);
            this.MBimu.TabIndex = 14;
            this.MBimu.Load += new System.EventHandler(this.MBimu_Load);
            this.MBimu.Click += new System.EventHandler(this.MBimu_Click);
            // 
            // MBpara
            // 
            this.MBpara.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.MBpara.Location = new System.Drawing.Point(10, 173);
            this.MBpara.Name = "MBpara";
            this.MBpara.Size = new System.Drawing.Size(98, 48);
            this.MBpara.TabIndex = 15;
            this.MBpara.Load += new System.EventHandler(this.MBpara_Load);
            this.MBpara.Click += new System.EventHandler(this.MBpara_Click);
            // 
            // MBremo
            // 
            this.MBremo.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.MBremo.Location = new System.Drawing.Point(10, 118);
            this.MBremo.Name = "MBremo";
            this.MBremo.Size = new System.Drawing.Size(98, 48);
            this.MBremo.TabIndex = 16;
            this.MBremo.Load += new System.EventHandler(this.MBremo_Load);
            this.MBremo.Click += new System.EventHandler(this.MBremo_Click);
            // 
            // MBgps
            // 
            this.MBgps.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.MBgps.Location = new System.Drawing.Point(10, 282);
            this.MBgps.Name = "MBgps";
            this.MBgps.Size = new System.Drawing.Size(98, 48);
            this.MBgps.TabIndex = 17;
            this.MBgps.Load += new System.EventHandler(this.MBgps_Load);
            this.MBgps.Click += new System.EventHandler(this.MBgps_Click);
            // 
            // label1
            // 
            this.label1.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(33)))), ((int)(((byte)(33)))));
            this.label1.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(199)))), ((int)(((byte)(191)))), ((int)(((byte)(191)))));
            this.label1.Location = new System.Drawing.Point(669, 93);
            this.label1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(40, 16);
            this.label1.TabIndex = 20;
            this.label1.Text = "串口";
            // 
            // label2
            // 
            this.label2.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(33)))), ((int)(((byte)(33)))));
            this.label2.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(199)))), ((int)(((byte)(191)))), ((int)(((byte)(191)))));
            this.label2.Location = new System.Drawing.Point(670, 146);
            this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(56, 16);
            this.label2.TabIndex = 21;
            this.label2.Text = "波特率";
            // 
            // tbBaudRate
            // 
            this.tbBaudRate.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.tbBaudRate.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(33)))), ((int)(((byte)(33)))));
            this.tbBaudRate.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.tbBaudRate.ForeColor = System.Drawing.SystemColors.ControlLight;
            this.tbBaudRate.Location = new System.Drawing.Point(670, 165);
            this.tbBaudRate.Margin = new System.Windows.Forms.Padding(2);
            this.tbBaudRate.Name = "tbBaudRate";
            this.tbBaudRate.Size = new System.Drawing.Size(76, 26);
            this.tbBaudRate.TabIndex = 22;
            this.tbBaudRate.Text = "115200";
            // 
            // cbComNum
            // 
            this.cbComNum.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.cbComNum.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(33)))), ((int)(((byte)(33)))));
            this.cbComNum.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.cbComNum.ForeColor = System.Drawing.SystemColors.ControlLight;
            this.cbComNum.FormattingEnabled = true;
            this.cbComNum.Location = new System.Drawing.Point(668, 113);
            this.cbComNum.Margin = new System.Windows.Forms.Padding(2);
            this.cbComNum.Name = "cbComNum";
            this.cbComNum.Size = new System.Drawing.Size(92, 24);
            this.cbComNum.TabIndex = 23;
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 1;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // label3
            // 
            this.label3.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.label3.AutoSize = true;
            this.label3.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(33)))), ((int)(((byte)(33)))));
            this.label3.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label3.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(199)))), ((int)(((byte)(191)))), ((int)(((byte)(191)))));
            this.label3.Location = new System.Drawing.Point(613, 360);
            this.label3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(72, 16);
            this.label3.TabIndex = 24;
            this.label3.Text = "电池电量";
            // 
            // pictureBox6
            // 
            this.pictureBox6.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.pictureBox6.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("pictureBox6.BackgroundImage")));
            this.pictureBox6.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pictureBox6.Location = new System.Drawing.Point(17, 352);
            this.pictureBox6.Margin = new System.Windows.Forms.Padding(2);
            this.pictureBox6.Name = "pictureBox6";
            this.pictureBox6.Size = new System.Drawing.Size(75, 30);
            this.pictureBox6.TabIndex = 26;
            this.pictureBox6.TabStop = false;
            // 
            // pictureBox5
            // 
            this.pictureBox5.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.pictureBox5.BackgroundImage = global::AppTest_V2._0.Properties.Resources.l4;
            this.pictureBox5.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pictureBox5.Location = new System.Drawing.Point(692, 351);
            this.pictureBox5.Margin = new System.Windows.Forms.Padding(2);
            this.pictureBox5.Name = "pictureBox5";
            this.pictureBox5.Size = new System.Drawing.Size(52, 30);
            this.pictureBox5.TabIndex = 25;
            this.pictureBox5.TabStop = false;
            // 
            // btreno
            // 
            this.btreno.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.btreno.BackgroundImage = global::AppTest_V2._0.Properties.Resources.reno_no;
            this.btreno.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.btreno.FlatAppearance.BorderSize = 0;
            this.btreno.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btreno.Location = new System.Drawing.Point(666, 282);
            this.btreno.Margin = new System.Windows.Forms.Padding(2);
            this.btreno.Name = "btreno";
            this.btreno.Size = new System.Drawing.Size(98, 48);
            this.btreno.TabIndex = 18;
            this.btreno.UseVisualStyleBackColor = true;
            this.btreno.Click += new System.EventHandler(this.btreno_Click);
            this.btreno.MouseLeave += new System.EventHandler(this.btreno_MouseLeave);
            this.btreno.MouseHover += new System.EventHandler(this.btreno_MouseHover);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("groupBox1.BackgroundImage")));
            this.groupBox1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.groupBox1.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.groupBox1.Location = new System.Drawing.Point(113, 42);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(2);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(2);
            this.groupBox1.Size = new System.Drawing.Size(551, 305);
            this.groupBox1.TabIndex = 5;
            this.groupBox1.TabStop = false;
            // 
            // pictureBox4
            // 
            this.pictureBox4.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.pictureBox4.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("pictureBox4.BackgroundImage")));
            this.pictureBox4.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pictureBox4.Location = new System.Drawing.Point(1, 346);
            this.pictureBox4.Margin = new System.Windows.Forms.Padding(2);
            this.pictureBox4.Name = "pictureBox4";
            this.pictureBox4.Size = new System.Drawing.Size(772, 44);
            this.pictureBox4.TabIndex = 3;
            this.pictureBox4.TabStop = false;
            // 
            // pictureBox2
            // 
            this.pictureBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.pictureBox2.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("pictureBox2.BackgroundImage")));
            this.pictureBox2.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.pictureBox2.Location = new System.Drawing.Point(1, 42);
            this.pictureBox2.Margin = new System.Windows.Forms.Padding(2);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(112, 304);
            this.pictureBox2.TabIndex = 1;
            this.pictureBox2.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.pictureBox1.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("pictureBox1.BackgroundImage")));
            this.pictureBox1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.pictureBox1.Location = new System.Drawing.Point(661, 42);
            this.pictureBox1.Margin = new System.Windows.Forms.Padding(2);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(112, 304);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // label4
            // 
            this.label4.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label4.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(199)))), ((int)(((byte)(191)))), ((int)(((byte)(191)))));
            this.label4.Location = new System.Drawing.Point(418, 360);
            this.label4.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(40, 16);
            this.label4.TabIndex = 27;
            this.label4.Text = "气压";
            // 
            // textBox1
            // 
            this.textBox1.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.textBox1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(33)))), ((int)(((byte)(33)))));
            this.textBox1.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(199)))), ((int)(((byte)(191)))), ((int)(((byte)(191)))));
            this.textBox1.Location = new System.Drawing.Point(172, 358);
            this.textBox1.Margin = new System.Windows.Forms.Padding(2);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBox1.Size = new System.Drawing.Size(76, 21);
            this.textBox1.TabIndex = 28;
            // 
            // label5
            // 
            this.label5.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label5.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(199)))), ((int)(((byte)(191)))), ((int)(((byte)(191)))));
            this.label5.Location = new System.Drawing.Point(131, 362);
            this.label5.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(40, 16);
            this.label5.TabIndex = 29;
            this.label5.Text = "帧率";
            // 
            // textBox2
            // 
            this.textBox2.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.textBox2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(33)))), ((int)(((byte)(33)))));
            this.textBox2.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(199)))), ((int)(((byte)(191)))), ((int)(((byte)(191)))));
            this.textBox2.Location = new System.Drawing.Point(474, 358);
            this.textBox2.Margin = new System.Windows.Forms.Padding(2);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(76, 21);
            this.textBox2.TabIndex = 30;
            // 
            // button1
            // 
            this.button1.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.button1.Location = new System.Drawing.Point(307, 356);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 31;
            this.button1.Text = "强行关闭";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // timer2
            // 
            this.timer2.Enabled = true;
            this.timer2.Interval = 1;
            this.timer2.Tick += new System.EventHandler(this.timer2_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoScroll = true;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(33)))), ((int)(((byte)(33)))));
            this.ClientSize = new System.Drawing.Size(774, 393);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.pictureBox6);
            this.Controls.Add(this.pictureBox5);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.cbComNum);
            this.Controls.Add(this.tbBaudRate);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btreno);
            this.Controls.Add(this.MBgps);
            this.Controls.Add(this.MBimu);
            this.Controls.Add(this.MBpara);
            this.Controls.Add(this.MBremo);
            this.Controls.Add(this.MBmain);
            this.Controls.Add(this.RBser);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.pictureBox4);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.pictureBox1);
            this.Margin = new System.Windows.Forms.Padding(2);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox6)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox5)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.PictureBox pictureBox4;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ImageList ILmain;
        private System.Windows.Forms.ImageList ILremo;
        private System.Windows.Forms.ImageList ILpara;
        private System.Windows.Forms.ImageList ILimu;
        private System.Windows.Forms.ImageList ILgps;
        private RichButton.RitchButton RBser;
        private System.Windows.Forms.ImageList ILser;
        private MutexButton.MutexButton MBmain;
        private MutexButton.MutexButton MBimu;
        private MutexButton.MutexButton MBpara;
        private MutexButton.MutexButton MBremo;
        private MutexButton.MutexButton MBgps;
        private System.Windows.Forms.Button btreno;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tbBaudRate;
        private System.Windows.Forms.ComboBox cbComNum;
        private System.IO.Ports.SerialPort serialPort;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.PictureBox pictureBox5;
        private System.Windows.Forms.PictureBox pictureBox6;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Timer timer2;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
    }
}

