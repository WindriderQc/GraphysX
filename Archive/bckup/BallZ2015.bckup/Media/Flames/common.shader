//#####################################################################//
//                     TrueVision8 Shader Script                       //
//#####################################################################//
//                                                                     //
//  Common Shader for Flames					       //
//								       //
// You have to define the used textures before creating the Shader     //
// At the left, the texture filename (can be in a PAK file)            //
// At the right, the texture name that will be used in the engine      //
//								       //	
//#####################################################################//

TEXTURES 17
Animations\Flames\flame1.jpg > Flame1
Animations\Flames\flame2.jpg > Flame2
Animations\Flames\flame3.jpg > Flame3
Animations\Flames\flame4.jpg > Flame4
Animations\Flames\flame5.jpg > Flame5
Animations\Flames\flame6.jpg > Flame6
Animations\Flames\flame7.jpg > Flame7
Animations\Flames\flame8.jpg > Flame8
Animations\Expl\expl1.jpg > Explosion1
Animations\Expl\expl2.jpg > Explosion2
Animations\Expl\expl3.jpg > Explosion3
Animations\Expl\expl4.jpg > Explosion4
Animations\Expl\expl5.jpg > Explosion5
Animations\Expl\expl6.jpg > Explosion6
Animations\Expl\expl7.jpg > Explosion7
Animations\Expl\expl8.jpg > Explosion8
Animations\Flames\flameball.jpg > FlameBall
glass.jpg > Glass


//#####################################
// Shaders Declarations. 
//#####################################

SHADER Flame1
{
//  Shader can contains several layers
   Layer map1
   {    

       //
       // Texture Animation :
       // Syntax : anim [FramePerSecond] [Tex1] [Tex2] ... [TexN] [0]
       //

         anim 10 Flame1 Flame2 Flame3 Flame4 Flame5 Flame6 Flame7 Flame8 [0]
        
       // Blender functions
       // Two ways : Blend [NO|ADD|ALPHA|COLOR|LIGHTMAP]
       //            BlendFunc [ZERO|ONE|ALPHA|INVALPHA|COLOR|INVCOLOR|DESTCOLOR] [ONE|ZERO|SRCCOLOR|ALPHA|INVALPHA|COLOR|INVCOLOR]
       //                             src blend                                    dest blend
      
	 blendFunc ONE ONE

    }

    Layer map2
    {
       // Other Tex Animation
       // for a blur effect
	
         anim 10 Flame2 Flame3 Flame4 Flame5 Flame6 Flame7 Flame8 Flame1 [0]

         blendFunc ONE ONE
    }

    Layer map3
    {
       // Finally blend another flame ball

        map FlameBall
        blendFunc ONE ONE
    }


}

//
// Explosion effect
//
SHADER Explosion
{   Layer map1
   {    

       //
       // Texture Animation :
       // Syntax : anim [FramePerSecond] [Tex1] [Tex2] ... [TexN] [0]
       //

         anim 10 Explosion1 Explosion2 Explosion3 Explosion4 Explosion5 Explosion6 Explosion7 Explosion8 [0]
	 blendFunc ONE ONE
         depthFunc NOWRITE

    }
}   


// ###########################################
//   Common Surfaces
// ###########################################

SHADER Glass
{
   $nolightmap
   Layer Glass1
   {
       map Glass
       alpha 0.5
       blendFunc ALPHA INVALPHA
       depthFunc NOWRITE
   }
}   

SHADER TexturedFace
{   Layer Lightmap
   {
       map $lightmap
       blend NO
   }
   Layer Texture
   {
       map $texture
       blend DESTCOLOR ZERO
   }
}