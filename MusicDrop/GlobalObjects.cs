using Microsoft.Xna.Framework.Media;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MusicDrop
{
    public sealed class GlobalObjects
    {
        private static readonly Lazy<GlobalObjects> _instance
            = new Lazy<GlobalObjects>(() => new GlobalObjects());

        private GlobalObjects()
        {
        }

        public static GlobalObjects Instance
        {
            get
            {
                return _instance.Value;
            }
        }

        public Song CurrentSong
        {
            get;
            set;
        }
    }
}
