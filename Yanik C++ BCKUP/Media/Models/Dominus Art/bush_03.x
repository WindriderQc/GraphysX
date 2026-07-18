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
Frame Axis {
   FrameTransformMatrix {
0.623477,-0.347316,-0.115873,0.000000,
-0.080775,0.092706,-0.712502,0.000000,
0.357114,0.627340,0.041140,0.000000,
0.537086,20.024803,0.106052,1.000000;;
 }
Frame Rectangle02 {
   FrameTransformMatrix {
0.876317,0.111197,0.468725,0.000000,
-0.036381,0.985492,-0.165775,0.000000,
-0.480359,0.128219,0.867649,0.000000,
0.000000,0.000000,0.000000,1.000000;;
 }
Mesh Rectangle021 {
 20;
-33.903133;0.000000;-31.339031;,
33.903133;0.000000;-31.339031;,
33.903133;0.000000;31.339031;,
-33.903133;0.000000;31.339031;,
-26.842928;-18.555908;-26.595339;,
25.691408;24.174608;-30.050261;,
44.764381;5.305659;26.595339;,
-7.769953;-37.424858;30.050261;,
-0.287077;-33.901920;-31.339031;,
0.287082;33.901920;-31.339031;,
0.287077;33.901920;31.339031;,
-0.287082;-33.901920;31.339031;,
-35.294071;12.859129;-26.523203;,
31.746401;3.800176;-31.127541;,
38.886223;27.336397;26.523205;,
-28.154251;36.395351;31.127542;,
3.269265;-30.328272;-30.321396;,
-18.279724;33.813183;-25.939058;,
-43.079952;21.637402;30.321394;,
-21.530964;-42.504051;25.939056;;

 10;
3;2,1,0;,
3;3,2,0;,
3;6,5,4;,
3;7,6,4;,
3;10,9,8;,
3;11,10,8;,
3;14,13,12;,
3;15,14,12;,
3;18,17,16;,
3;19,18,16;;
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
"bush3.tga";
}
 }
}

 MeshNormals {
 20;
0.000000;1.000000;0.000000;,
0.000000;1.000000;0.000000;,
0.000000;1.000000;0.000000;,
0.000000;1.000000;0.000000;,
-0.554194;0.715708;0.425007;,
-0.554194;0.715708;0.425007;,
-0.554194;0.715708;0.425007;,
-0.554194;0.715708;0.425007;,
-0.999964;0.008468;-0.000000;,
-0.999964;0.008468;-0.000000;,
-0.999964;0.008468;-0.000000;,
-0.999964;0.008468;-0.000000;,
0.097386;0.917139;-0.386488;,
0.097386;0.917139;-0.386488;,
0.097386;0.917139;-0.386488;,
0.097386;0.917139;-0.386488;,
-0.861651;-0.259690;-0.436027;,
-0.861651;-0.259690;-0.436027;,
-0.861651;-0.259690;-0.436027;,
-0.861651;-0.259690;-0.436027;;

 10;
3;2,1,0;,
3;3,2,0;,
3;6,5,4;,
3;7,6,4;,
3;10,9,8;,
3;11,10,8;,
3;14,13,12;,
3;15,14,12;,
3;18,17,16;,
3;19,18,16;;
 }
MeshTextureCoords {
 20;
0.995573;1.990542;,
1.995573;1.990542;,
1.995573;0.995751;,
0.997845;0.998023;,
-0.993972;1.995996;,
0.001478;1.993695;,
-0.001031;0.992746;,
-0.994209;0.997319;,
-0.998619;0.994944;,
-0.003772;0.996391;,
-0.006500;0.004435;,
-0.985447;0.005260;,
0.999848;0.987872;,
1.997794;0.989662;,
1.995974;-0.005768;,
1.004844;-0.003015;,
0.002427;-0.001215;,
0.998740;-0.005312;,
1.008964;-0.995816;,
-0.000978;-1.003076;;
}
}
 }
 }
