namespace CommunicationDemo
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
            this.freshBt = new System.Windows.Forms.Button();
            this.comBox = new System.Windows.Forms.ComboBox();
            this.openBt = new System.Windows.Forms.Button();
            this.text = new System.Windows.Forms.TextBox();
            this.sendBt = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.getBox = new System.Windows.Forms.TextBox();
            this.getData = new System.Windows.Forms.Timer(this.components);
            this.SuspendLayout();
            // 
            // freshBt
            // 
            this.freshBt.Location = new System.Drawing.Point(240, 83);
            this.freshBt.Name = "freshBt";
            this.freshBt.Size = new System.Drawing.Size(121, 23);
            this.freshBt.TabIndex = 0;
            this.freshBt.Text = "刷新串口";
            this.freshBt.UseVisualStyleBackColor = true;
            this.freshBt.Click += new System.EventHandler(this.freshBt_Click);
            // 
            // comBox
            // 
            this.comBox.FormattingEnabled = true;
            this.comBox.Location = new System.Drawing.Point(92, 86);
            this.comBox.Name = "comBox";
            this.comBox.Size = new System.Drawing.Size(121, 20);
            this.comBox.TabIndex = 1;
            // 
            // openBt
            // 
            this.openBt.Location = new System.Drawing.Point(240, 112);
            this.openBt.Name = "openBt";
            this.openBt.Size = new System.Drawing.Size(121, 23);
            this.openBt.TabIndex = 2;
            this.openBt.Text = "打开串口";
            this.openBt.UseVisualStyleBackColor = true;
            this.openBt.Click += new System.EventHandler(this.openBt_Click);
            // 
            // text
            // 
            this.text.Location = new System.Drawing.Point(92, 188);
            this.text.Name = "text";
            this.text.Size = new System.Drawing.Size(121, 21);
            this.text.TabIndex = 3;
            // 
            // sendBt
            // 
            this.sendBt.Location = new System.Drawing.Point(240, 186);
            this.sendBt.Name = "sendBt";
            this.sendBt.Size = new System.Drawing.Size(121, 23);
            this.sendBt.TabIndex = 4;
            this.sendBt.Text = "发送";
            this.sendBt.UseVisualStyleBackColor = true;
            this.sendBt.Click += new System.EventHandler(this.sendBt_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(141, 248);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(72, 16);
            this.label1.TabIndex = 5;
            this.label1.Text = "接收数据";
            // 
            // getBox
            // 
            this.getBox.Location = new System.Drawing.Point(240, 243);
            this.getBox.Name = "getBox";
            this.getBox.Size = new System.Drawing.Size(121, 21);
            this.getBox.TabIndex = 6;
            // 
            // getData
            // 
            this.getData.Enabled = true;
            this.getData.Tick += new System.EventHandler(this.getData_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(495, 382);
            this.Controls.Add(this.getBox);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.sendBt);
            this.Controls.Add(this.text);
            this.Controls.Add(this.openBt);
            this.Controls.Add(this.comBox);
            this.Controls.Add(this.freshBt);
            this.Name = "Form1";
            this.Text = "窗体1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button freshBt;
        private System.Windows.Forms.ComboBox comBox;
        private System.Windows.Forms.Button openBt;
        private System.Windows.Forms.TextBox text;
        private System.Windows.Forms.Button sendBt;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox getBox;
        private System.Windows.Forms.Timer getData;
    }
}

