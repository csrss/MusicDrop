using Microsoft.Xna.Framework.Media;
using Microsoft.Xna.Framework.Media.PhoneExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MusicDrop
{
    public class HttpSongsExposeEvent : Http.IHttpSongsExposeEvent
    {
        public virtual IList<Http.CAudioItem> GetAudioCollection()
        {
            using (MediaLibrary pMediaLibrary = new MediaLibrary())
            {
                IList<Http.CAudioItem> pAudioItems = new List<Http.CAudioItem>();
                foreach (Song pSong in pMediaLibrary.Songs)
                {
                    Http.CAudioItem pAudioItem = new Http.CAudioItem(pSong.Name, pSong.Artist.Name, pSong.Album.Name);
                    pAudioItems.Add(pAudioItem);
                }

                if (pAudioItems.Count > 0)
                {
                    return pAudioItems;
                }
                return null;
            }
        }
    }
}
