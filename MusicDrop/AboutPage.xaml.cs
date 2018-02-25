using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using System.Xml.Linq;
using Microsoft.Phone.Tasks;
using MusicDrop.Resources;

namespace MusicDrop
{
    public partial class AboutPage : PhoneApplicationPage
    {
        public AboutPage()
        {
            InitializeComponent();

            XElement xe = XElement.Load("WMAppManifest.xml");
            var attr =
               (from manifest in xe.Descendants("App") select manifest).SingleOrDefault();
            if (attr != null)
            {
                this.UIAppVersion.Text = AppResources.IDS_About_page_version_label + " " + attr.Attribute("Version").Value;
            }
        }

        private void HyperlinkButton_Click(object sender, RoutedEventArgs e)
        {
            MarketplaceReviewTask review = new MarketplaceReviewTask();
            review.Show();
        }
    }
}