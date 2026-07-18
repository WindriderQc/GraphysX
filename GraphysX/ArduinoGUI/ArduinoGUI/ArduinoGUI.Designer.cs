namespace GraphysX
{
    partial class ArduinoGUI
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.bSetPin = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.checkPinState = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.labelDistance = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.labelReceivedMsg = new System.Windows.Forms.Label();
            this.bSonicScan = new System.Windows.Forms.Button();
            this.textBoxSerial = new System.Windows.Forms.TextBox();
            this.bUp = new System.Windows.Forms.Button();
            this.bLeft = new System.Windows.Forms.Button();
            this.bStop = new System.Windows.Forms.Button();
            this.bRight = new System.Windows.Forms.Button();
            this.comboBoxPort = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // bSetPin
            // 
            this.bSetPin.Location = new System.Drawing.Point(12, 21);
            this.bSetPin.Name = "bSetPin";
            this.bSetPin.Size = new System.Drawing.Size(75, 23);
            this.bSetPin.TabIndex = 0;
            this.bSetPin.Text = "Set Pin";
            this.bSetPin.UseVisualStyleBackColor = true;
            this.bSetPin.Click += new System.EventHandler(this.bSetPin_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(94, 23);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(34, 20);
            this.textBox1.TabIndex = 2;
            this.textBox1.Text = "8";
            // 
            // checkPinState
            // 
            this.checkPinState.AutoSize = true;
            this.checkPinState.Location = new System.Drawing.Point(135, 26);
            this.checkPinState.Name = "checkPinState";
            this.checkPinState.Size = new System.Drawing.Size(59, 17);
            this.checkPinState.TabIndex = 3;
            this.checkPinState.Text = "On/Off";
            this.checkPinState.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(16, 132);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(85, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Sonic Distance :";
            // 
            // labelDistance
            // 
            this.labelDistance.AutoSize = true;
            this.labelDistance.Location = new System.Drawing.Point(107, 132);
            this.labelDistance.Name = "labelDistance";
            this.labelDistance.Size = new System.Drawing.Size(35, 13);
            this.labelDistance.TabIndex = 5;
            this.labelDistance.Text = "label2";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(16, 157);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(85, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "Received Msg : ";
            // 
            // labelReceivedMsg
            // 
            this.labelReceivedMsg.AutoSize = true;
            this.labelReceivedMsg.Location = new System.Drawing.Point(107, 157);
            this.labelReceivedMsg.Name = "labelReceivedMsg";
            this.labelReceivedMsg.Size = new System.Drawing.Size(16, 13);
            this.labelReceivedMsg.TabIndex = 7;
            this.labelReceivedMsg.Text = "...";
            // 
            // bSonicScan
            // 
            this.bSonicScan.Location = new System.Drawing.Point(12, 199);
            this.bSonicScan.Name = "bSonicScan";
            this.bSonicScan.Size = new System.Drawing.Size(75, 23);
            this.bSonicScan.TabIndex = 8;
            this.bSonicScan.Text = "Sonic Scan";
            this.bSonicScan.UseVisualStyleBackColor = true;
            this.bSonicScan.Click += new System.EventHandler(this.bSonicScan_Click);
            // 
            // textBoxSerial
            // 
            this.textBoxSerial.Location = new System.Drawing.Point(11, 265);
            this.textBoxSerial.Multiline = true;
            this.textBoxSerial.Name = "textBoxSerial";
            this.textBoxSerial.Size = new System.Drawing.Size(411, 161);
            this.textBoxSerial.TabIndex = 9;
            // 
            // bUp
            // 
            this.bUp.Location = new System.Drawing.Point(43, 62);
            this.bUp.Name = "bUp";
            this.bUp.Size = new System.Drawing.Size(26, 23);
            this.bUp.TabIndex = 10;
            this.bUp.Text = "^";
            this.bUp.UseVisualStyleBackColor = true;
            this.bUp.Click += new System.EventHandler(this.bUp_Click);
            // 
            // bLeft
            // 
            this.bLeft.Location = new System.Drawing.Point(11, 78);
            this.bLeft.Name = "bLeft";
            this.bLeft.Size = new System.Drawing.Size(26, 23);
            this.bLeft.TabIndex = 11;
            this.bLeft.Text = "<";
            this.bLeft.UseVisualStyleBackColor = true;
            this.bLeft.Click += new System.EventHandler(this.bLeft_Click);
            // 
            // bStop
            // 
            this.bStop.Location = new System.Drawing.Point(43, 91);
            this.bStop.Name = "bStop";
            this.bStop.Size = new System.Drawing.Size(26, 23);
            this.bStop.TabIndex = 12;
            this.bStop.Text = "x";
            this.bStop.UseVisualStyleBackColor = true;
            this.bStop.Click += new System.EventHandler(this.bStop_Click);
            // 
            // bRight
            // 
            this.bRight.Location = new System.Drawing.Point(75, 78);
            this.bRight.Name = "bRight";
            this.bRight.Size = new System.Drawing.Size(26, 23);
            this.bRight.TabIndex = 13;
            this.bRight.Text = ">";
            this.bRight.UseVisualStyleBackColor = true;
            this.bRight.Click += new System.EventHandler(this.bRight_Click);
            // 
            // comboBoxPort
            // 
            this.comboBoxPort.FormattingEnabled = true;
            this.comboBoxPort.Location = new System.Drawing.Point(12, 238);
            this.comboBoxPort.Name = "comboBoxPort";
            this.comboBoxPort.Size = new System.Drawing.Size(121, 21);
            this.comboBoxPort.TabIndex = 16;
            this.comboBoxPort.SelectedValueChanged += new System.EventHandler(this.comboBoxPort_SelectedValueChanged);
            // 
            // ArduinoGUI
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(435, 438);
            this.Controls.Add(this.comboBoxPort);
            this.Controls.Add(this.bRight);
            this.Controls.Add(this.bStop);
            this.Controls.Add(this.bLeft);
            this.Controls.Add(this.bUp);
            this.Controls.Add(this.textBoxSerial);
            this.Controls.Add(this.bSonicScan);
            this.Controls.Add(this.labelReceivedMsg);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.labelDistance);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.checkPinState);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.bSetPin);
            this.Name = "ArduinoGUI";
            this.Text = "ArduinoGUI";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ArduinoGUI_FormClosing);
            this.Load += new System.EventHandler(this.ArduinoGUI_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Button bSetPin;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.CheckBox checkPinState;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label labelDistance;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label labelReceivedMsg;
        private System.Windows.Forms.Button bSonicScan;
        private System.Windows.Forms.TextBox textBoxSerial;
        private System.Windows.Forms.Button bUp;
        private System.Windows.Forms.Button bLeft;
        private System.Windows.Forms.Button bStop;
        private System.Windows.Forms.Button bRight;
        private System.Windows.Forms.ComboBox comboBoxPort;
    }
}

