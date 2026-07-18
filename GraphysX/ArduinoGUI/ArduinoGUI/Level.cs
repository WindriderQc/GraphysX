using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GraphysX
{
    public class Level
    {
        public Level() { }
        public Level(string xmlFile,  string asciiFile, bool baddFloor,  string floorTex, string floorNormals, string sky, string ringsFile,  int numHuman, string screenshot)
        {
            XMLFile = xmlFile;
            ASCIIFilePath = asciiFile;
            bAddFloor = baddFloor;
            FloorTex = floorTex;
            FloorNormals = floorNormals;
            SkyDay = sky; 
            RingsFile = ringsFile;
            iNumHuman = numHuman;
            Screenshot = screenshot;
        }
        
        public string XMLFile = "..\\StockRoom\\Suzanne1.xml";
        public string ASCIIFilePath = "..\\StockRoom\\Suzanne1.ASCII";
        public bool bAddFloor = true;
        public string FloorTex = "concrete.png";
        public string FloorNormals = "";
        public string RingsFile = "..\\StockRoom\\Suzanne1.obj";
        public int iNumHuman = 100;
        public int nbrTour = 3;
        public string Screenshot = "Suzanne1.jpg";
        public string SkyDay = "SkyX";

        public const float ScoreGold = 100;
        public const float ScoreSilver = 75;
        public const float ScoreBronze = 50;
        public float ScoreBest = ScoreGold;
    }
   
}