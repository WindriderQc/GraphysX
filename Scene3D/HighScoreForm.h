#pragma once
#include "Player.h"
#include "HighScoreData.h"

namespace BallZ {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace System::Collections::Generic;
	using namespace System::Xml::Serialization;
	using namespace System::IO;                 // File System Methods



	//typedef HighScoreData HSData;

	/// <summary>
	/// Summary for HighScoreForm
	/// </summary>
	public ref class HighScoreForm : public System::Windows::Forms::Form
	{
	public:
		HighScoreForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

		}
		HighScoreForm(float time)
		{
			InitializeComponent();
			fWinnerTime = time;
			//TopTenList = gcnew List<Player^>();
			
			// Check to see if the save exists
			if (!File::Exists(FileName))
			{
				//If the file doesn't exist, make a fake one...
				// Create the data to save
				HighScoreData^ data = gcnew HighScoreData(4);
				data->PlayerName[0] = "Jo Lamotte";
				data->Level[0] = 10;
				data->Score[0] = 200500;

				data->PlayerName[1] = "LesKargau";
				data->Level[1] = 10;
				data->Score[1] = 187000;

				data->PlayerName[2] = "Les NoName";
				data->Level[2] = 9;
				data->Score[2] = 113300;

				data->PlayerName[3] = "SpeedyJonhy";
				data->Level[3] = 1;
				data->Score[3] = 1000;

				SaveHighScores(data, FileName);
			}

			/*// Write in XML
			XmlSerializer^ serializer = gcnew XmlSerializer(TopTenList->GetType());
			StreamReader^ reader = File::OpenText("HighScore.xml");
			TopTenList = (List<Player^>^)serializer->Deserialize(reader);
			reader->Close();*/
			

		}
		//List<Player^>^ TopTenList;
		String^ FileName = "HighScores.xml";
		//HighScoreData^ HighScore;
		static void SaveHighScores(HighScoreData^ data, String^ filename)
		{
			// Open the file, creating it if necessary
			FileStream^ stream = File::Open(filename, FileMode::OpenOrCreate);
			try
			{
				// Convert the object to XML data and put it in the stream
				XmlSerializer^ serializer = gcnew XmlSerializer(data->GetType());
				serializer->Serialize(stream, data);
			}
			finally
			{
				// Close the file
				stream->Close();
			}
		}
		static HighScoreData^ LoadHighScores(String^ filename)
		{
			HighScoreData^ data = gcnew HighScoreData();
			// Open the file
			FileStream^ stream = File::Open(filename, FileMode::OpenOrCreate,
				FileAccess::Read);
			try
			{
				// Read the data from the file
				XmlSerializer^ serializer = gcnew XmlSerializer(data->GetType());
				data = (HighScoreData^)serializer->Deserialize(stream);
			}
			finally
			{
				// Close the file
				stream->Close();
			}

			return (data);
		}
	private:
		void SaveHighScore()
		{

			//TopTenList->Add(gcnew Player(textBox1->Text, iWinnerTime));

		
			// Create the data to save
			HighScoreData^ data = LoadHighScores(FileName);
			
			int scoreIndex = -1;
			for (int i = 0; i < data->Count; i++)
			{
				if (fWinnerTime < data->Score[i])
				{
					scoreIndex = i;
					break;
				}
			}

			if (scoreIndex > -1)
			{
				//New high score found ... do swaps
				for (int i = data->Count - 1; i > scoreIndex; i--)
				{
					data->PlayerName[i] = data->PlayerName[i - 1];
					data->Score[i] = data->Score[i - 1];
					data->Level[i] = data->Level[i - 1];
				}

				data->PlayerName[scoreIndex] = textBox1->Text; //Retrieve User Name Here
				data->Score[scoreIndex] = fWinnerTime;
				//data->Level[scoreIndex] = currentLevel + 1;

				SaveHighScores(data, FileName);
				textBox2->Clear();
				for (unsigned int i = 0; i < sizeof(data); i++)
				{
					textBox2->Text += data->PlayerName[i] + ".....";
					textBox2->Text += data->Score[i] +"\r\n";

				}
			}
		}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~HighScoreForm()
		{
			if (components)
			{
				delete components;
			}
		} 
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::Button^  button1;
	protected:

	private:
		float fWinnerTime;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(47, 21);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(356, 20);
			this->textBox1->TabIndex = 0;
			this->textBox1->Text = L"-- Enter ton nom man! --";
			this->textBox1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &HighScoreForm::textBox1_TextChanged);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->textBox2);
			this->groupBox1->Location = System::Drawing::Point(47, 80);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(356, 297);
			this->groupBox1->TabIndex = 1;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Top Ten";
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(29, 24);
			this->textBox2->Multiline = true;
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(296, 257);
			this->textBox2->TabIndex = 0;
			this->textBox2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(76, 46);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(296, 25);
			this->button1->TabIndex = 2;
			this->button1->Text = L"Save";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &HighScoreForm::button1_Click);
			// 
			// HighScoreForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(465, 402);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->textBox1);
			this->Name = L"HighScoreForm";
			this->Text = L"HighScoreForm";
			this->TopMost = true;
			this->Load += gcnew System::EventHandler(this, &HighScoreForm::HighScoreForm_Load);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

		SaveHighScore();

		/*// Write in XML
		StreamWriter^ xmlFile = File::CreateText("HighScore.xml");
		XmlSerializer^ serializer = gcnew XmlSerializer(TopTenList->GetType());
		serializer->Serialize(xmlFile, TopTenList);
		xmlFile->Close();*/
	}
private: System::Void HighScoreForm_Load(System::Object^  sender, System::EventArgs^  e) {
	
	HighScoreData^ data = LoadHighScores(FileName);
	for (UINT i = 0; i < sizeof(data); i++)
	{
		textBox2->Text += data->PlayerName[i] + ".....";
		textBox2->Text += data->Score[i] + "\r\n";

	}
}
};
}
