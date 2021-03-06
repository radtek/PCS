USE [ChangKeMes]
GO

CREATE TABLE [dbo].[PCS_Base_SerialNumber](
	[UID] INT IDENTITY(1,1) NOT NULL PRIMARY KEY,
	[Type] INT NULL,
	[Name] NVARCHAR(50) NULL,
	[Desc] NVARCHAR(255) NULL,
	[Prefix] NVARCHAR(50) NULL,
	[SerialRule] NVARCHAR(50) NULL,
	[LastNumber] INT NOT NULL DEFAULT 1,
	[LastDateTime] DATETIME NULL,
	[LastSerialNumber] NVARCHAR(50) NULL,
	[Sep] NVARCHAR(10) NULL,
	[NumFill] CHAR(1) NOT NULL DEFAULT '0',
	[NumBase] INT NOT NULL DEFAULT 10,
	[State] INT NOT NULL DEFAULT 1,
)
GO

SET IDENTITY_INSERT [dbo].[PCS_Base_SerialNumber] ON
INSERT [dbo].[PCS_Base_SerialNumber] ([UID], [Type], [Name], [Desc], [Prefix], [SerialRule], [LastNumber], [LastDateTime], [LastSerialNumber], [Sep], [NumFill], [NumBase], [State]) VALUES (1, 100, N'WorkShopCode', N'车间编号', N'WS', N'P|N:3', 1, NULL, NULL, N'.', N'0', 10, 1)
INSERT [dbo].[PCS_Base_SerialNumber] ([UID], [Type], [Name], [Desc], [Prefix], [SerialRule], [LastNumber], [LastDateTime], [LastSerialNumber], [Sep], [NumFill], [NumBase], [State]) VALUES (2, 101, N'WorkLineCode', N'产线编号', N'WL', N'P|N:3', 1, NULL, NULL, N'.', N'0', 10, 1)
INSERT [dbo].[PCS_Base_SerialNumber] ([UID], [Type], [Name], [Desc], [Prefix], [SerialRule], [LastNumber], [LastDateTime], [LastSerialNumber], [Sep], [NumFill], [NumBase], [State]) VALUES (3, 102, N'WorkStationCode', N'工位编号', N'ST', N'P|N:4', 1, NULL, NULL, N'.', N'0', 10, 1)
INSERT [dbo].[PCS_Base_SerialNumber] ([UID], [Type], [Name], [Desc], [Prefix], [SerialRule], [LastNumber], [LastDateTime], [LastSerialNumber], [Sep], [NumFill], [NumBase], [State]) VALUES (4, 103, N'CraftCode', N'工艺编号', N'CF', N'P|N:4', 1, NULL, NULL, N'.', N'0', 10, 1)
INSERT [dbo].[PCS_Base_SerialNumber] ([UID], [Type], [Name], [Desc], [Prefix], [SerialRule], [LastNumber], [LastDateTime], [LastSerialNumber], [Sep], [NumFill], [NumBase], [State]) VALUES (5, 104, N'MeasureCode', N'测试编号', N'MS', N'P|N:4', 1, NULL, NULL, N'.', N'0', 10, 1)
INSERT [dbo].[PCS_Base_SerialNumber] ([UID], [Type], [Name], [Desc], [Prefix], [SerialRule], [LastNumber], [LastDateTime], [LastSerialNumber], [Sep], [NumFill], [NumBase], [State]) VALUES (6, 105, N'MaterialCode', N'物料编号', N'MT', N'P|N:4', 1, NULL, NULL, N'.', N'0', 10, 1)
INSERT [dbo].[PCS_Base_SerialNumber] ([UID], [Type], [Name], [Desc], [Prefix], [SerialRule], [LastNumber], [LastDateTime], [LastSerialNumber], [Sep], [NumFill], [NumBase], [State]) VALUES (7, 106, N'OrderCode', N'工单编号', N'WO', N'P|D:l|N:4', 1, NULL, NULL, N'.', N'0', 10, 1)
INSERT [dbo].[PCS_Base_SerialNumber] ([UID], [Type], [Name], [Desc], [Prefix], [SerialRule], [LastNumber], [LastDateTime], [LastSerialNumber], [Sep], [NumFill], [NumBase], [State]) VALUES (8, 107, N'FixtureCode', N'工装编号', N'FX', N'P|N:4', 1, NULL, NULL, N'.', N'0', 10, 1)
INSERT [dbo].[PCS_Base_SerialNumber] ([UID], [Type], [Name], [Desc], [Prefix], [SerialRule], [LastNumber], [LastDateTime], [LastSerialNumber], [Sep], [NumFill], [NumBase], [State]) VALUES (9, 108, N'WorkerCode', N'员工编号', N'WK', N'P|N:4', 1, NULL, NULL, N'.', N'0', 10, 1)
INSERT [dbo].[PCS_Base_SerialNumber] ([UID], [Type], [Name], [Desc], [Prefix], [SerialRule], [LastNumber], [LastDateTime], [LastSerialNumber], [Sep], [NumFill], [NumBase], [State]) VALUES (10, 109, N'ProductCode', N'产品编号', N'PT', N'P|N:4', 1, NULL, NULL, N'.', N'0', 10, 1)
SET IDENTITY_INSERT [dbo].[PCS_Base_SerialNumber] OFF
