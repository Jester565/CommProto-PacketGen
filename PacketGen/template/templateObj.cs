using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Comnet;
using Comnet.Serialization;

namespace NGCP
{
  public class ^Name^ : ObjSerializable
  {
    ^Constructor^

    public override void Input(ObjectStream obj)
    {
      ^PackBody^
    }

    public override void Output(ObjectStream obj)
    {
      ^UnpackBody^
    }

    public override ObjSerializable Create()
    {
      return new ^CreateConstruct^
    }

    #region Data
    ^Data^
    #endregion
  }
}