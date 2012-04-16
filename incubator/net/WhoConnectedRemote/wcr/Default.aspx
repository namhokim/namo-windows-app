<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Default.aspx.cs" Inherits="wcr._Default" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>누가 원격데스크탑을 하고 있나?</title>
</head>
<body>
    <h1>원격데스크탑 접속자</h1>
    <form id="form1" runat="server">
    <div>
        <asp:ListView runat="server" ID="list"></asp:ListView>
    </div>
    </form>
</body>
</html>
