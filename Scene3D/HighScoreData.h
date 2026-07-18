#pragma once


[Serializable]
public value class HighScoreData
{
public:
	array<String^>^ PlayerName;
	array<float>^ Score;
	array<int>^ Level;

	int Count;

	HighScoreData(int count)
	{
		PlayerName = gcnew array<String^>(count);
		Score = gcnew array<float>(count);
		Level = gcnew array<int>(count);

		Count = count;
	}
};