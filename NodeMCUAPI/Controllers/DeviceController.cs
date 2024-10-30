using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using NodeMCUAPI.Model;
using MySql.Data.MySqlClient;
using System.Data;
using NodeMCUAPI;

namespace NodeMCUAPI.Controllers
{
    [Route("[controller]")]
    public class DeviceController : Controller
    {
        private string connectionString = "Server=127.0.0.1;Port=3306;Database=nodedb;Uid=root;password=####";

        // GET Function
        [HttpGet]
        public IEnumerable<Device> Get()
        {
            IList<Device> ListaDispositivos = new List<Device>();

            MySqlConnection conexion = new MySqlConnection(connectionString);
            conexion.Open();

            MySqlCommand cmd = new MySqlCommand();
            cmd.CommandType = CommandType.StoredProcedure;
            cmd.CommandText = "sp_get_devices";
            cmd.Connection = conexion;

            Device device1 = new Device();

            using (var reader = cmd.ExecuteReader())
            {
                while (reader.Read())
                {
                    device1 = new Device();
                    device1.deviceId = Convert.ToInt32(reader["device_id"]);
                    device1.deviceName = reader["device_name"].ToString();
                    ListaDispositivos.Add(device1);
                }
            }
            conexion.Dispose();

            return ListaDispositivos;
        }


        // POST Function
        [HttpPost]
        public void Post(Device deviceItem)
        {
            MySqlConnection conexion = new MySqlConnection(connectionString);
            conexion.Open();

            MySqlCommand cmd = new MySqlCommand();
            cmd.CommandType = CommandType.StoredProcedure;
            cmd.CommandText = "sp_add_device";
            cmd.Parameters.AddWithValue("@d_device_id", deviceItem.deviceId);
            cmd.Parameters.AddWithValue("@d_device_name", deviceItem.deviceName);
            cmd.Connection = conexion;

            MySqlDataReader myReader = cmd.ExecuteReader();

            conexion.Close();
        }
    }
}