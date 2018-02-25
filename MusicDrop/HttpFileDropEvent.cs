using Microsoft.Xna.Framework.Media;
using Microsoft.Xna.Framework.Media.PhoneExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.System.Display;

namespace MusicDrop
{
    public class HttpFileDropEvent : Http.IHttpFileDropEvent
    {
        public virtual Http.CSongTransferResult OnFileDrop(String strFilePathName)
        {
            using (MediaLibrary pMediaLibrary = new MediaLibrary())
            {
                // create track location Uri
                Uri trackLocation = new Uri(strFilePathName, UriKind.Relative);

                try
                {
                    // Save media file to a local media library
                    Song song = pMediaLibrary.SaveSong(trackLocation, null, SaveSongOperation.MoveToLibrary);
                    if (null != song)
                    {
                        return new Http.CSongTransferResult(song.Name, song.Artist.Name, song.Album.Name);
                    }
                }
                catch (Exception)
                {
                }
                return null;
            }
        }

        //public virtual void OnFileDelete(String name, String artist, String album)
        //{
        //    using (MediaLibrary pMediaLibrary = new MediaLibrary())
        //    {
        //        try
        //        {
        //            Song song = FindSong(pMediaLibrary, name, artist, album);
        //            if (song != null)
        //            {
        //                pMediaLibrary.Delete(song);
        //            }
        //        }
        //        catch (Exception)
        //        { 
        //        }
        //    }
        //}


        public virtual void OnFilePlay(String name, String artist, String album)
        {
            using (MediaLibrary pMediaLibrary = new MediaLibrary())
            {
                try
                {
                    Song song = FindSong(pMediaLibrary, name, artist, album);
                    if (song != null)
                    {
                        if (null != GlobalObjects.Instance.CurrentSong && 
                            song == GlobalObjects.Instance.CurrentSong && 
                            MediaPlayer.State == MediaState.Playing)
                        {
                            GlobalObjects.Instance.CurrentSong = null;
                            MediaPlayer.Stop();
                        }
                        else
                        {
                            GlobalObjects.Instance.CurrentSong = song;
                            MediaPlayer.Play(song);
                        }
                    }
                }
                catch (Exception)
                { 
                }
            }
        }

        private Song FindSong(MediaLibrary pMediaLibrary, String name, String artist, String album)
        {
            Song song = null;
            foreach (Song pSong in pMediaLibrary.Songs)
            {
                Boolean found = false;
                if (pSong.Name.CompareTo(name) == 0)
                {
                    found = true;
                }
                if (!String.IsNullOrEmpty(artist))
                {
                    if (pSong.Artist.Name.CompareTo(artist) != 0)
                    {
                        found = false;
                    }
                    if (!String.IsNullOrEmpty(album))
                    {
                        if (pSong.Album.Name.CompareTo(album) != 0)
                        {
                            found = false;
                        }
                    }
                }

                if (found)
                {
                    song = pSong;
                    break;
                }
            }
            return song;
        }
    }
}
