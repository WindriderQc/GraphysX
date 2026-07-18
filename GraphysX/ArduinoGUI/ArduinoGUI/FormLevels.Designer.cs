namespace ArduinoGUI
{
    partial class FormLevels
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormLevels));
            this.label1 = new System.Windows.Forms.Label();
            this.pictureBoxScene = new System.Windows.Forms.PictureBox();
            this.pictureBoxPrev = new System.Windows.Forms.PictureBox();
            this.pictureBoxNext = new System.Windows.Forms.PictureBox();
            this.labelNbrLap = new System.Windows.Forms.Label();
            this.labelSkyName = new System.Windows.Forms.Label();
            this.labelNbrHuman = new System.Windows.Forms.Label();
            this.labelBestScore = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxScene)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxPrev)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxNext)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Papyrus", 48F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.SystemColors.Highlight;
            this.label1.Location = new System.Drawing.Point(-7, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(438, 100);
            this.label1.TabIndex = 0;
            this.label1.Text = "Select Level :";
            // 
            // pictureBoxScene
            // 
            this.pictureBoxScene.Image = ((System.Drawing.Image)(resources.GetObject("pictureBoxScene.Image")));
            this.pictureBoxScene.Location = new System.Drawing.Point(238, 141);
            this.pictureBoxScene.Name = "pictureBoxScene";
            this.pictureBoxScene.Size = new System.Drawing.Size(574, 326);
            this.pictureBoxScene.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBoxScene.TabIndex = 1;
            this.pictureBoxScene.TabStop = false;
            this.pictureBoxScene.Click += new System.EventHandler(this.pictureBoxScene_Click);
            // 
            // pictureBoxPrev
            // 
            this.pictureBoxPrev.Image = ((System.Drawing.Image)(resources.GetObject("pictureBoxPrev.Image")));
            this.pictureBoxPrev.Location = new System.Drawing.Point(167, 158);
            this.pictureBoxPrev.Name = "pictureBoxPrev";
            this.pictureBoxPrev.Size = new System.Drawing.Size(65, 295);
            this.pictureBoxPrev.TabIndex = 2;
            this.pictureBoxPrev.TabStop = false;
            this.pictureBoxPrev.Click += new System.EventHandler(this.pictureBoxPrev_Click);
            // 
            // pictureBoxNext
            // 
            this.pictureBoxNext.Image = ((System.Drawing.Image)(resources.GetObject("pictureBoxNext.Image")));
            this.pictureBoxNext.Location = new System.Drawing.Point(818, 157);
            this.pictureBoxNext.Name = "pictureBoxNext";
            this.pictureBoxNext.Size = new System.Drawing.Size(71, 296);
            this.pictureBoxNext.TabIndex = 3;
            this.pictureBoxNext.TabStop = false;
            this.pictureBoxNext.Click += new System.EventHandler(this.pictureBoxNext_Click);
            // 
            // labelNbrLap
            // 
            this.labelNbrLap.AutoSize = true;
            this.labelNbrLap.Font = new System.Drawing.Font("Papyrus", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelNbrLap.ForeColor = System.Drawing.SystemColors.Highlight;
            this.labelNbrLap.Location = new System.Drawing.Point(12, 507);
            this.labelNbrLap.Name = "labelNbrLap";
            this.labelNbrLap.Size = new System.Drawing.Size(160, 51);
            this.labelNbrLap.TabIndex = 4;
            this.labelNbrLap.Text = "Nbr Lap:";
            // 
            // labelSkyName
            // 
            this.labelSkyName.AutoSize = true;
            this.labelSkyName.Font = new System.Drawing.Font("Papyrus", 32F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelSkyName.ForeColor = System.Drawing.SystemColors.ControlDarkDark;
            this.labelSkyName.Location = new System.Drawing.Point(437, 70);
            this.labelSkyName.Name = "labelSkyName";
            this.labelSkyName.Size = new System.Drawing.Size(146, 68);
            this.labelSkyName.TabIndex = 5;
            this.labelSkyName.Text = "Name";
            // 
            // labelNbrHuman
            // 
            this.labelNbrHuman.AutoSize = true;
            this.labelNbrHuman.Font = new System.Drawing.Font("Papyrus", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelNbrHuman.ForeColor = System.Drawing.SystemColors.Highlight;
            this.labelNbrHuman.Location = new System.Drawing.Point(12, 567);
            this.labelNbrHuman.Name = "labelNbrHuman";
            this.labelNbrHuman.Size = new System.Drawing.Size(204, 51);
            this.labelNbrHuman.TabIndex = 6;
            this.labelNbrHuman.Text = "Nbr Human:";
            // 
            // labelBestScore
            // 
            this.labelBestScore.AutoSize = true;
            this.labelBestScore.Font = new System.Drawing.Font("Papyrus", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelBestScore.ForeColor = System.Drawing.SystemColors.Highlight;
            this.labelBestScore.Location = new System.Drawing.Point(12, 627);
            this.labelBestScore.Name = "labelBestScore";
            this.labelBestScore.Size = new System.Drawing.Size(212, 51);
            this.labelBestScore.TabIndex = 7;
            this.labelBestScore.Text = "Best Score:";
            // 
            // FormLevels
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1092, 697);
            this.Controls.Add(this.labelBestScore);
            this.Controls.Add(this.labelNbrHuman);
            this.Controls.Add(this.labelSkyName);
            this.Controls.Add(this.labelNbrLap);
            this.Controls.Add(this.pictureBoxNext);
            this.Controls.Add(this.pictureBoxPrev);
            this.Controls.Add(this.pictureBoxScene);
            this.Controls.Add(this.label1);
            this.Name = "FormLevels";
            this.Text = "FormLevels";
            this.Shown += new System.EventHandler(this.FormLevels_Shown);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxScene)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxPrev)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxNext)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.PictureBox pictureBoxScene;
        private System.Windows.Forms.PictureBox pictureBoxPrev;
        private System.Windows.Forms.PictureBox pictureBoxNext;
        private System.Windows.Forms.Label labelNbrLap;
        private System.Windows.Forms.Label labelSkyName;
        private System.Windows.Forms.Label labelNbrHuman;
        protected System.Windows.Forms.Label labelBestScore;
    }
}