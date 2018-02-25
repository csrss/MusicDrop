using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using MusicDrop.Resources;
using System.IO.IsolatedStorage;
using System.Windows.Resources;
using System.Windows.Threading;
using Microsoft.Phone.Info;
using System.Windows.Media.Imaging;
using Microsoft.Phone.Net.NetworkInformation;

namespace MusicDrop
{
    public partial class MainPage : PhoneApplicationPage
    {
        // Constructor
        public MainPage()
        {
            InitializeComponent();

            // Sample code to localize the ApplicationBar
            BuildLocalizedApplicationBar();

            try
            {
                WwwRoot wwwRoot = new WwwRoot(AppResources.IDS_Server_source_path, "html");
                wwwRoot.Extract();

                Microsoft.Phone.Shell.PhoneApplicationService.Current.ApplicationIdleDetectionMode
                    = Microsoft.Phone.Shell.IdleDetectionMode.Disabled;

                HttpServerAsync pHttpServer = (Application.Current as App).HttpServer = new HttpServerAsync();
                if (null != pHttpServer)
                {
                    pHttpServer.RunWorkerAsync();

                    this.UIServerInfo.Text =
                        String.Format("http://{0}:{1}/", pHttpServer.Address, pHttpServer.Port.ToString());
                }
            }
            catch (Exception ex)
            {
                this.UIImageStatus.Source = new BitmapImage(new Uri(@"Assets/ApplicationIconWarning.png", UriKind.RelativeOrAbsolute));

                this.UIManualLink.Content = AppResources.IDS_Main_page_errors_link_content;
                this.UIManualLink.NavigateUri = new Uri(AppResources.IDS_Main_page_errors_link);
                
                this.UIStatus.Text = String.Format("{0} (HRESULT: 0x{1:X8})", ex.Message, ex.HResult);
            }
        }

        // Sample code for building a localized ApplicationBar
        private void BuildLocalizedApplicationBar()
        {
            // Set the page's ApplicationBar to a new instance of ApplicationBar.
            ApplicationBar = new ApplicationBar();
            ApplicationBar.Opacity = 0.5;

            // Create a new button and set the text value to the localized string from AppResources.
            ApplicationBarIconButton appBarButton = new ApplicationBarIconButton(new Uri("/Assets/appbar.info.png", UriKind.Relative));
            appBarButton.Text = AppResources.IDS_Main_page_info_button_text;
            appBarButton.Click += appBarButton_Click;
            ApplicationBar.Buttons.Add(appBarButton);
        }

        void appBarButton_Click(object sender, EventArgs e)
        {
            this.NavigationService.Navigate(new Uri("/AboutPage.xaml", UriKind.RelativeOrAbsolute));
        }
    }
}