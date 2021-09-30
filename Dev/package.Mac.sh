export PKG_CONFIG_PATH=/Library/Frameworks/Mono.framework/Versions/Current/lib/pkgconfig
export AS="as -arch i386"
export CC="clang -arch i386 -mmacosx-version-min=10.6"

mkdir Mac/Effekseer.app/Contents/Resources/

# (cd release;
# mkbundle  Effekseer.exe --deps -L './';
# otool -L Effekseer;)

# (cd release;
# mkbundle -o tools/mqoToEffekseerModelConverter tools/mqoToEffekseerModelConverter.exe --deps;
# otool -L tools/mqoToEffekseerModelConverter;)

cp release/Effekseer Mac/Effekseer.app/Contents/Resources/
cp release/Effekseer.exe Mac/Effekseer.app/Contents/Resources/
cp release/libViewer.dylib Mac/Effekseer.app/Contents/Resources/
cp release/EffekseerCore.dll Mac/Effekseer.app/Contents/Resources/

cp -r release/resources Mac/Effekseer.app/Contents/Resources/
cp -r release/scripts Mac/Effekseer.app/Contents/Resources/

mkdir Mac/Effekseer.app/Contents/Resources/tools
cp release/tools/mqoToEffekseerModelConverter Mac/Effekseer.app/Contents/Resources/tools
# echo Please download from https://github.com/effekseer/Effekseer/releases/download/Prebuild/ to Prebuild

cp release/tools//fbxToEffekseerModelConverter Mac/Effekseer.app/Contents/Resources/tools/
cp release/tools//libfbxsdk.dylib  Mac/Effekseer.app/Contents/Resources/tools/

chmod +x Mac/Effekseer.app/Contents/MacOS/script.sh
