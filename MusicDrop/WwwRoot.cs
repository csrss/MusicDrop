using System;
using System.Collections.Generic;
using System.IO;
using System.IO.IsolatedStorage;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Resources;

namespace MusicDrop
{
    class WwwRoot
    {
        public WwwRoot(String sourceFolder, String targetFolder)
        {
            if (Directory.Exists(sourceFolder))
            {
                SourceFolder = sourceFolder;
            }
            else 
            {
                throw new DirectoryNotFoundException(sourceFolder);
            }

            // If target folder has been specified
            if (!String.IsNullOrEmpty(targetFolder))
            {
                // And if exists
                if (Directory.Exists(sourceFolder))
                {
                    // Then we set it up.
                    TargetFolder = targetFolder;
                }
                else
                {
                    // If it wpecified and not exists, we just throw an exception.
                    throw new DirectoryNotFoundException(targetFolder);
                }
            }
            else 
            {
                // If target is null or empty, then target is the same as source.
                TargetFolder = SourceFolder;
            }
        }

        public String SourceFolder
        {
            get;
            private set;
        }

        public String TargetFolder
        {
            get;
            private set;
        }

        public void Extract()
        {
            using (IsolatedStorageFile storage = IsolatedStorageFile.GetUserStoreForApplication())
            {
                // If target folder does not exists, we create it. Target folder is "html";
                if (false == storage.DirectoryExists(TargetFolder))
                {
                    storage.CreateDirectory(TargetFolder);
                }

                // Files returned have source folder path appended to the beginning.
                foreach (String sourceFile in Directory.GetFiles(SourceFolder))
                {
                    // file in target folder, with target folder path appended to the beginning
                    String targetFile = TargetFolder + "\\" + Path.GetFileName(sourceFile);

                    // In debug, if target file exists, we replace it with current. In release, we just skip it.
                    if (storage.FileExists(targetFile))
                    {
                        storage.DeleteFile(targetFile);
                    }

                    // Get resource stream from source file.
                    StreamResourceInfo resource = Application.GetResourceStream(new Uri(sourceFile, UriKind.Relative));
                    using (IsolatedStorageFileStream fileStream = storage.CreateFile(targetFile))
                    {
                        int chunkSize = 4096;
                        byte[] bytes = new byte[chunkSize];
                        int byteCount;
                        while ((byteCount = resource.Stream.Read(bytes, 0, chunkSize)) > 0)
                        {
                            fileStream.Write(bytes, 0, byteCount);
                        }
                    }
                }
            }
        }
    }
}
