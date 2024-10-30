using System;
namespace NodeMCUAPI.Model
{
    public class Temperature
    {
        public int logId { get; set; }
        public DateTime LogDate { get; set; }
        public int deviceId { get; set; }
        public float temp { get; set; }
    }
}