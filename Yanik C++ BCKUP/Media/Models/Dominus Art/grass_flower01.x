xof 0302txt 0032
Header {
 1;
 0;
 1;
}
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template FrameTransformMatrix {
 <F6F23F41-7686-11cf-8F52-0040333594A3>
 Matrix4x4 frameMatrix;
}

template Frame {
 <3D82AB46-62DA-11cf-AB39-0020AF71E433>
 [...]
}
template FloatKeys {
 <10DD46A9-775B-11cf-8F52-0040333594A3>
 DWORD nValues;
 array FLOAT values[nValues];
}

template TimedFloatKeys {
 <F406B180-7B3B-11cf-8F52-0040333594A3>
 DWORD time;
 FloatKeys tfkeys;
}

template AnimationKey {
 <10DD46A8-775B-11cf-8F52-0040333594A3>
 DWORD keyType;
 DWORD nKeys;
 array TimedFloatKeys keys[nKeys];
}

template AnimationOptions {
 <E2BF56C0-840F-11cf-8F52-0040333594A3>
 DWORD openclosed;
 DWORD positionquality;
}

template Animation {
 <3D82AB4F-62DA-11cf-AB39-0020AF71E433>
 [...]
}

template AnimationSet {
 <3D82AB50-62DA-11cf-AB39-0020AF71E433>
 [Animation]
}

template XSkinMeshHeader {
 <3cf169ce-ff7c-44ab-93c0-f78f62d172e2>
 WORD nMaxSkinWeightsPerVertex;
 WORD nMaxSkinWeightsPerFace;
 WORD nBones;
}

template VertexDuplicationIndices {
 <b8d65549-d7c9-4995-89cf-53a9a8b031e3>
 DWORD nIndices;
 DWORD nOriginalVertices;
 array DWORD indices[nIndices];
}

template SkinWeights {
 <6f0d123b-bad2-4167-a0d0-80224f25fabb>
 STRING transformNodeName;
 DWORD nWeights;
 array DWORD vertexIndices[nWeights];
 array FLOAT weights[nWeights];
 Matrix4x4 matrixOffset;
}
Frame flower_stem01a01 {
   FrameTransformMatrix {
0.515197,0.000000,0.000000,0.000000,
0.000000,0.000000,-0.493007,0.000000,
0.000000,0.332690,0.000000,0.000000,
5.475524,16.104868,-0.719825,1.000000;;
 }
Mesh flower_stem01a011 {
 16;
-50.000000;0.000000;-50.000000;,
-50.000000;0.000000;50.000000;,
21.454750;0.000000;50.000000;,
21.454750;0.000000;-50.000000;,
-50.000000;0.000000;-50.000000;,
21.454750;0.000000;-50.000000;,
21.454750;0.000000;50.000000;,
-50.000000;0.000000;50.000000;,
-10.204980;-39.795017;-50.000000;,
-10.204983;-39.795021;50.000000;,
-10.204986;31.659737;50.000004;,
-10.204983;31.659740;-49.999996;,
-10.204980;-39.795017;-50.000000;,
-10.204983;31.659740;-49.999996;,
-10.204986;31.659737;50.000004;,
-10.204983;-39.795021;50.000000;;

 8;
3;2,1,0;,
3;0,3,2;,
3;6,5,4;,
3;4,7,6;,
3;10,9,8;,
3;8,11,10;,
3;14,13,12;,
3;12,15,14;;
MeshMaterialList {
 1;
 8;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
Material {
 0.588000;0.588000;0.588000;1.000000;;
4.000000;
 0.900000;0.900000;0.900000;;
 0.117600;0.117600;0.117600;;
TextureFilename {
"plant01_stem.tga";
}
 }
}

 MeshNormals {
 16;
0.000000;-1.000000;0.000000;,
0.000000;-1.000000;0.000000;,
0.000000;-1.000000;0.000000;,
0.000000;-1.000000;0.000000;,
0.000000;1.000000;0.000000;,
0.000000;1.000000;0.000000;,
0.000000;1.000000;0.000000;,
0.000000;1.000000;0.000000;,
1.000000;0.000000;0.000000;,
1.000000;0.000000;0.000000;,
1.000000;0.000000;0.000000;,
1.000000;0.000000;0.000000;,
-1.000000;-0.000000;-0.000000;,
-1.000000;-0.000000;-0.000000;,
-1.000000;-0.000000;-0.000000;,
-1.000000;-0.000000;-0.000000;;

 8;
3;2,1,0;,
3;0,3,2;,
3;6,5,4;,
3;4,7,6;,
3;10,9,8;,
3;8,11,10;,
3;14,13,12;,
3;12,15,14;;
 }
MeshTextureCoords {
 16;
0.000500;0.999500;,
0.000500;0.000500;,
0.999501;0.000500;,
0.999500;0.999501;,
0.000500;0.999500;,
0.999500;0.999501;,
0.999501;0.000500;,
0.000500;0.000500;,
0.000500;0.999500;,
0.000500;0.000500;,
0.999501;0.000500;,
0.999500;0.999501;,
0.000500;0.999500;,
0.999500;0.999501;,
0.999501;0.000500;,
0.000500;0.000500;;
}
}
 }
Frame Axis {
   FrameTransformMatrix {
0.297835,-0.052002,-0.014072,0.000000,
-0.053148,-0.296546,-0.029035,0.000000,
-0.008799,0.031042,-0.300943,0.000000,
2.824158,40.511543,0.770696,1.000000;;
 }
Frame flower_petals_blue01 {
   FrameTransformMatrix {
0.985319,0.170699,-0.002810,0.000000,
-0.169976,0.982414,0.077276,0.000000,
0.015952,-0.075664,0.997006,0.000000,
0.000000,0.000000,0.000000,1.000000;;
 }
Mesh flower_petals_blue02 {
 12;
38.293045;6.244056;0.000000;,
11.833200;6.244056;36.418850;,
0.000000;29.973984;-0.000000;,
-30.979725;6.244056;22.508083;,
-30.979721;6.244056;-22.508091;,
11.833206;6.244056;-36.418846;,
11.833200;6.244056;36.418850;,
38.293045;6.244056;0.000000;,
0.000000;29.973984;-0.000000;,
-30.979725;6.244056;22.508083;,
-30.979721;6.244056;-22.508091;,
11.833206;6.244056;-36.418846;;

 10;
3;2,1,0;,
3;2,3,1;,
3;2,4,3;,
3;2,5,4;,
3;2,0,5;,
3;8,7,6;,
3;8,6,9;,
3;8,9,10;,
3;8,10,11;,
3;8,11,7;;
MeshMaterialList {
 1;
 10;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
Material {
 0.588235;0.588235;0.588235;1.000000;;
4.000000;
 0.900000;0.900000;0.900000;;
 0.117647;0.117647;0.117647;;
TextureFilename {
"plant01_flower_blue.tga";
}
 }
}

 MeshNormals {
 12;
0.526751;0.850020;0.000000;,
0.162775;0.850020;0.500970;,
0.000000;1.000000;-0.000000;,
-0.426151;0.850020;0.309616;,
-0.426151;0.850020;-0.309617;,
0.162775;0.850020;-0.500970;,
-0.162775;-0.850020;-0.500970;,
-0.526751;-0.850020;-0.000000;,
-0.000000;-1.000000;0.000000;,
0.426151;-0.850020;-0.309616;,
0.426151;-0.850020;0.309617;,
-0.162775;-0.850020;0.500970;;

 10;
3;2,1,0;,
3;2,3,1;,
3;2,4,3;,
3;2,5,4;,
3;2,0,5;,
3;8,7,6;,
3;8,6,9;,
3;8,9,10;,
3;8,10,11;,
3;8,11,7;;
 }
MeshTextureCoords {
 12;
0.903661;0.889135;,
1.020188;0.205115;,
0.458540;0.487069;,
0.446137;-0.042976;,
-0.025172;0.487716;,
0.257593;1.063792;,
1.020188;0.205115;,
0.903661;0.889135;,
0.458540;0.487069;,
0.446137;-0.042976;,
-0.025172;0.487716;,
0.257593;1.063792;;
}
}
 }
 }
