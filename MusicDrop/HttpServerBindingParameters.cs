using MusicDrop.Resources;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Networking;
using Windows.Networking.Connectivity;

namespace MusicDrop
{
    class HttpServerBindingParameters
    {
        public HttpServerBindingParameters()
        {
            String message = String.Empty;
            IReadOnlyList<HostName> hostnames = NetworkInformation.GetHostNames();
            foreach (HostName hostname in hostnames)
            {
                bool valid = false;
                uint nIanaInterfaceType = 0;

                IPAddress = GetValidRawIP(hostname, out nIanaInterfaceType, out valid);

                if (nIanaInterfaceType == 71)
                {
                    if (!valid)
                    {
                        if (!String.IsNullOrEmpty(IPAddress))
                        {
                            message += String.Format(AppResources.IDS_Error_valid_iface_wrong_ip, IPAddress) + Environment.NewLine;
                        }
                        else
                        {
                            message += AppResources.IDS_Error_valid_iface_no_ip + Environment.NewLine;
                        }

                        continue;
                    }

                    Port = 19123;
                    return;
                }
                else
                {
                    message += String.Format(AppResources.IDS_Error_invalid_iface, IPAddress, nIanaInterfaceType) + Environment.NewLine;
                }
            }
            if (!String.IsNullOrEmpty(message))
            {
                throw new Exception(message);
            }
            throw new Exception(AppResources.IDS_Error_network_not_available);
        }

        public String IPAddress
        {
            get;
            private set;
        }

        public int Port
        {
            get;
            private set;
        }

        private bool IsIPv4(String value)
        {
            var quads = value.Split('.');

            // if we do not have 4 quads, return false
            if (!(quads.Length == 4)) return false;

            // for each quad
            foreach (var quad in quads)
            {
                int q;
                // if parse fails 
                // or length of parsed int != length of quad string (i.e.; '1' vs '001')
                // or parsed int < 0
                // or parsed int > 255
                // return false
                if (!Int32.TryParse(quad, out q)
                    || !q.ToString().Length.Equals(quad.Length)
                    || q < 0
                    || q > 255) { return false; }

            }

            return true;
        }

        private String GetValidRawIP(HostName hostname, out uint ianaType, out bool valid)
        {
            valid = false;
            ianaType = hostname.IPInformation.NetworkAdapter.IanaInterfaceType;
            if (!String.IsNullOrEmpty(hostname.CanonicalName) && IsIPv4(hostname.CanonicalName))
            {
                valid = true;
                return hostname.CanonicalName;
            }
            else if (!String.IsNullOrEmpty(hostname.DisplayName) && IsIPv4(hostname.DisplayName))
            {
                valid = true;
                return hostname.DisplayName;
            }
            else if (!String.IsNullOrEmpty(hostname.RawName) && IsIPv4(hostname.RawName))
            {
                valid = true;
                return hostname.RawName;
            }
            else
            {
                return !String.IsNullOrEmpty(hostname.CanonicalName) ? hostname.CanonicalName
                        : !String.IsNullOrEmpty(hostname.DisplayName) ? hostname.DisplayName
                        : !String.IsNullOrEmpty(hostname.RawName) ? hostname.RawName
                        : String.Empty;
            }
        }
    }
}
