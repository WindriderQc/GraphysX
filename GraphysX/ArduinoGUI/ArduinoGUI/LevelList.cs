using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Xml.Serialization;
using System.IO;


namespace GraphysX
{
    public class LevelList
    {
        public LevelList()
        {
           
        }


        public List<Level> levels = new List<Level>();
        public static int iSelected = -1;


        public void Serialize()
        {
            // Write in XML
            string path = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            StreamWriter  xmlFile = File.CreateText(path + "\\..\\Stockroom\\levelList.xml");  
            XmlSerializer serializer = new XmlSerializer(this.GetType());
            serializer.Serialize(xmlFile, this);
            xmlFile.Close();
        }
        public void Deserialized()
	    {
            try
            {
                string path = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);

                using (StreamReader xmlFile = File.OpenText(path + "\\..\\Stockroom\\levelList.xml"))
                {

                    XmlSerializer serializer = new XmlSerializer(this.GetType());
                    LevelList t = (LevelList)serializer.Deserialize(xmlFile);
                    xmlFile.Close();

                    foreach (Level l in t.levels)
                    {
                        this.levels.Add(l);
                    }
                }
            }
            catch (Exception e)
            {
                System.Windows.Forms.MessageBox.Show(e.Message);
                Console.Write("LevelList.xml not found.  Default Levels and Scores loaded.\n");
                CreateDefaultLevels();
            }
        }

        void CreateDefaultLevels()
        {
            Level l = new Level("", "..\\StockRoom\\Level1_base.ASCII"
                                     , true, "\\Texture\\Alien\\Alien01_B_diff.bmp", "\\Texture\\Alien\\Alien01_B_normal.bmp"
                                     , "ClearBlue"
                                     , "", 0, "screenShotLevel1.png");
            levels.Add(l);

            l = new Level("", "..\\StockRoom\\Level2_base.ASCII"
                                  , true, "Checkerboard.png", ""
                                  , "LostValley"
                                  , "", 10, "screenShotLevel2.png");
            levels.Add(l);

            l = new Level("", "..\\StockRoom\\Level3_base.ASCII"
                                  , true, "\\Texture\\Alien\\Alien02_diff.bmp", "\\Texture\\Alien\\Alien02_normal.bmp"
                                  , "NightSky"
                                  , "", 0, "screenShotLevel3.png");
            levels.Add(l);
        }
    }
}
