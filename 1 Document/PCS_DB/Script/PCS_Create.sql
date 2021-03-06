USE [master]
GO
/****** Object:  Database [PCS]    Script Date: 04/02/2018 17:51:59 ******/
CREATE DATABASE [PCS] ON  PRIMARY 
( NAME = N'PCS', FILENAME = N'D:\DATA\PCS.mdf' , SIZE = 3072KB , MAXSIZE = UNLIMITED, FILEGROWTH = 1024KB )
 LOG ON 
( NAME = N'PCS_log', FILENAME = N'D:\DATA\PCS_log.ldf' , SIZE = 1024KB , MAXSIZE = 2048GB , FILEGROWTH = 10%)
GO
ALTER DATABASE [PCS] SET COMPATIBILITY_LEVEL = 100
GO
IF (1 = FULLTEXTSERVICEPROPERTY('IsFullTextInstalled'))
begin
EXEC [PCS].[dbo].[sp_fulltext_database] @action = 'enable'
end
GO
ALTER DATABASE [PCS] SET ANSI_NULL_DEFAULT OFF
GO
ALTER DATABASE [PCS] SET ANSI_NULLS OFF
GO
ALTER DATABASE [PCS] SET ANSI_PADDING OFF
GO
ALTER DATABASE [PCS] SET ANSI_WARNINGS OFF
GO
ALTER DATABASE [PCS] SET ARITHABORT OFF
GO
ALTER DATABASE [PCS] SET AUTO_CLOSE OFF
GO
ALTER DATABASE [PCS] SET AUTO_CREATE_STATISTICS ON
GO
ALTER DATABASE [PCS] SET AUTO_SHRINK OFF
GO
ALTER DATABASE [PCS] SET AUTO_UPDATE_STATISTICS ON
GO
ALTER DATABASE [PCS] SET CURSOR_CLOSE_ON_COMMIT OFF
GO
ALTER DATABASE [PCS] SET CURSOR_DEFAULT  GLOBAL
GO
ALTER DATABASE [PCS] SET CONCAT_NULL_YIELDS_NULL OFF
GO
ALTER DATABASE [PCS] SET NUMERIC_ROUNDABORT OFF
GO
ALTER DATABASE [PCS] SET QUOTED_IDENTIFIER OFF
GO
ALTER DATABASE [PCS] SET RECURSIVE_TRIGGERS OFF
GO
ALTER DATABASE [PCS] SET  DISABLE_BROKER
GO
ALTER DATABASE [PCS] SET AUTO_UPDATE_STATISTICS_ASYNC OFF
GO
ALTER DATABASE [PCS] SET DATE_CORRELATION_OPTIMIZATION OFF
GO
ALTER DATABASE [PCS] SET TRUSTWORTHY OFF
GO
ALTER DATABASE [PCS] SET ALLOW_SNAPSHOT_ISOLATION OFF
GO
ALTER DATABASE [PCS] SET PARAMETERIZATION SIMPLE
GO
ALTER DATABASE [PCS] SET READ_COMMITTED_SNAPSHOT OFF
GO
ALTER DATABASE [PCS] SET HONOR_BROKER_PRIORITY OFF
GO
ALTER DATABASE [PCS] SET  READ_WRITE
GO
ALTER DATABASE [PCS] SET RECOVERY FULL
GO
ALTER DATABASE [PCS] SET  MULTI_USER
GO
ALTER DATABASE [PCS] SET PAGE_VERIFY CHECKSUM
GO
ALTER DATABASE [PCS] SET DB_CHAINING OFF
GO
EXEC sys.sp_db_vardecimal_storage_format N'PCS', N'ON'
GO
USE [PCS]
GO
/****** Object:  Table [dbo].[PCS_WorkOrder]    Script Date: 04/02/2018 17:52:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PCS_WorkOrder](
	[UID] [int] IDENTITY(1,1) NOT NULL,
	[WorkShopID] [nvarchar](50) NULL,
	[WorkLineID] [nvarchar](50) NULL,
	[OrderID] [nvarchar](50) NULL,
	[ProductionBatch] [nvarchar](50) NULL,
	[CraftID] [nvarchar](50) NULL,
	[PlanDate] [datetime] NULL,
	[PlanProductionQuantity] [int] NULL,
	[PlanInspectionQuantity] [int] NULL,
	[FirstPassQuantity] [int] NULL,
	[UnqualifiedQuantity] [int] NULL,
	[QualifiedQuantity] [int] NULL,
	[InspectionQuantity] [int] NULL,
	[RepairQuantity] [int] NULL,
	[PackageQuantity] [int] NULL,
	[StartTime] [datetime] NULL,
	[FinishTime] [datetime] NULL,
	[CreateUser] [nvarchar](50) NULL,
	[CreateTime] [datetime] NULL,
	[ModifyUser] [nvarchar](50) NULL,
	[ModifyTime] [datetime] NULL,
	[State] [int] NULL,
	[Description] [nvarchar](255) NULL,
 CONSTRAINT [PK_PCS_WORKORDER] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PCS_Operator_Hours]    Script Date: 04/02/2018 17:52:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PCS_Operator_Hours](
	[UID] [int] IDENTITY(1,1) NOT NULL,
	[WorkShopID] [nvarchar](50) NULL,
	[WorkLineID] [nvarchar](50) NULL,
	[WorkStationID] [nvarchar](50) NULL,
	[OrderID] [nvarchar](50) NULL,
	[WorkerID] [nvarchar](50) NULL,
	[LogMarker] [nvarchar](50) NULL,
	[LogTime] [datetime] NULL,
	[LogState] [int] NULL,
	[UploadTime] [datetime] NULL,
 CONSTRAINT [PK_PCS_OPERATOR_HOURS] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PCS_Material_Batch]    Script Date: 04/02/2018 17:52:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PCS_Material_Batch](
	[UID] [int] IDENTITY(1,1) NOT NULL,
	[WorkShopID] [nvarchar](50) NULL,
	[WorkLineID] [nvarchar](50) NULL,
	[WorkStationID] [nvarchar](50) NULL,
	[OrderID] [nvarchar](50) NULL,
	[MaterialID] [nvarchar](50) NULL,
	[MaterialBatch] [nvarchar](50) NULL,
	[PackageBatch] [nvarchar](100) NULL,
	[BatchQuantity] [int] NULL,
	[BatchState] [int] NULL,
 CONSTRAINT [PK_PCS_MATERIAL_BATCH] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PCS_Data_Transfer]    Script Date: 04/02/2018 17:52:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PCS_Data_Transfer](
	[UID] [int] IDENTITY(1,1) NOT NULL,
	[AssemblyID] [nvarchar](50) NULL,
	[OrderID] [nvarchar](50) NULL,
	[WorkStationID] [nvarchar](50) NULL,
	[TransferBarcode] [nvarchar](50) NULL,
	[AssemblyState] [int] NULL,
	[RepairState] [int] NULL,
	[RetestState] [int] NULL,
	[CreateTime] [datetime] NULL,
	[FinishTime] [datetime] NULL,
 CONSTRAINT [PK_PCS_DATA_TRANSFER] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PCS_Data_Station_Measure]    Script Date: 04/02/2018 17:52:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PCS_Data_Station_Measure](
	[UID] [int] IDENTITY(1,1) NOT NULL,
	[AssemblyID] [nvarchar](50) NOT NULL,
	[OrderID] [nvarchar](50) NULL,
	[WorkStationID] [nvarchar](50) NOT NULL,
	[MeasureType] [nvarchar](50) NOT NULL,
	[MeasureOrder] [int] NULL,
	[MeasureValue] [money] NULL,
	[MeasureState] [int] NULL,
	[RetestCount] [int] NULL,
	[UploadTime] [datetime] NULL,
 CONSTRAINT [PK_PCS_DATA_STATION_MEASURE] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PCS_Data_Station_Material]    Script Date: 04/02/2018 17:52:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PCS_Data_Station_Material](
	[UID] [int] IDENTITY(1,1) NOT NULL,
	[AssemblyID] [nvarchar](50) NULL,
	[OrderID] [nvarchar](50) NULL,
	[WorkStationID] [nvarchar](50) NULL,
	[MaterialID] [nvarchar](50) NULL,
	[MaterialBarcode] [nvarchar](50) NULL,
	[MaterialBatch] [nvarchar](50) NULL,
	[RepairCount] [int] NULL,
	[UploadTime] [datetime] NULL,
 CONSTRAINT [PK_PCS_DATA_STATION_MATERIAL] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PCS_Data_Station]    Script Date: 04/02/2018 17:52:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PCS_Data_Station](
	[UID] [int] IDENTITY(1,1) NOT NULL,
	[AssemblyID] [nvarchar](50) NULL,
	[OrderID] [nvarchar](50) NULL,
	[WorkStationID] [nvarchar](50) NULL,
	[WorkerID] [nvarchar](50) NULL,
	[AssemblyState] [int] NULL,
	[RepairState] [int] NULL,
	[RetestState] [int] NULL,
	[CreateTime] [datetime] NULL,
	[FinishTime] [datetime] NULL,
	[UploadTime] [datetime] NULL,
 CONSTRAINT [PK_PCS_DATA_STATION] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PCS_Data_Repair]    Script Date: 04/02/2018 17:52:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PCS_Data_Repair](
	[UID] [int] IDENTITY(1,1) NOT NULL,
	[MaterialID] [nvarchar](50) NOT NULL,
	[MaterialBarcode] [nvarchar](50) NOT NULL,
	[RepairCount] [int] NOT NULL,
	[State] [int] NULL,
	[WorkerID] [nvarchar](50) NULL,
	[CreateTime] [datetime] NULL,
	[FinishTime] [datetime] NULL,
	[UploadTime] [datetime] NULL,
 CONSTRAINT [PK_PCS_DATA_REPAIR] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PCS_Data_Package_Assmebly]    Script Date: 04/02/2018 17:52:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PCS_Data_Package_Assmebly](
	[UID] [int] IDENTITY(1,1) NOT NULL,
	[PackageID] [nvarchar](50) NULL,
	[AssemblyID] [nvarchar](50) NULL,
	[UploadTime] [datetime] NULL,
 CONSTRAINT [PK_PCS_DATA_PACKAGE_ASSMEBLY] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PCS_Data_Package]    Script Date: 04/02/2018 17:52:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PCS_Data_Package](
	[UID] [int] IDENTITY(1,1) NOT NULL,
	[WorkShopID] [nvarchar](50) NULL,
	[WorkLineID] [nvarchar](50) NULL,
	[PackageID] [nvarchar](50) NULL,
	[OrderID] [nvarchar](50) NULL,
	[ProductID] [nvarchar](50) NULL,
	[WorkerID] [nvarchar](50) NULL,
	[PackageBarcode] [nvarchar](50) NULL,
	[PackageSerial] [int] NULL,
	[PackageWeight] [money] NULL,
	[PackageQuantity] [int] NULL,
	[PackageState] [int] NULL,
	[CreateTime] [datetime] NULL,
	[FinishTime] [datetime] NULL,
	[UploadTime] [datetime] NULL,
 CONSTRAINT [PK_PCS_DATA_PACKAGE] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PCS_Data_Assembly_Relation]    Script Date: 04/02/2018 17:52:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PCS_Data_Assembly_Relation](
	[UID] [int] IDENTITY(1,1) NOT NULL,
	[AssemblyID] [nvarchar](50) NULL,
	[SubAssemblyID] [nvarchar](50) NULL,
	[UploadTime] [datetime] NULL,
 CONSTRAINT [PK_PCS_DATA_ASSEMBLY_RELATION] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PCS_Data_Assembly]    Script Date: 04/02/2018 17:52:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PCS_Data_Assembly](
	[UID] [int] IDENTITY(1,1) NOT NULL,
	[WorkShopID] [nvarchar](50) NULL,
	[WorkLineID] [nvarchar](50) NULL,
	[AssemblyID] [nvarchar](50) NULL,
	[OrderID] [nvarchar](50) NULL,
	[ProductID] [nvarchar](50) NULL,
	[WorkerID] [nvarchar](50) NULL,
	[AssemblyBarcode] [nvarchar](50) NULL,
	[AssemblySerial] [int] NULL,
	[AssemblyState] [int] NULL,
	[InspectionMarker] [int] NULL,
	[RepairMarker] [int] NULL,
	[RetestState] [int] NULL,
	[CreateTime] [datetime] NULL,
	[FinishTime] [datetime] NULL,
	[UploadTime] [datetime] NULL,
 CONSTRAINT [PK_PCS_DATA_ASSEMBLY] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PCS_Craft_Station_Measure]    Script Date: 04/02/2018 17:52:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PCS_Craft_Station_Measure](
	[UID] [int] IDENTITY(1,1) NOT NULL,
	[CraftID] [nvarchar](50) NULL,
	[WorkStationID] [nvarchar](50) NULL,
	[MeasureType] [nvarchar](50) NULL,
	[MeasureOrder] [int] NULL,
	[StandardValue] [money] NULL,
	[UpperLimit] [money] NULL,
	[LowerLimit] [money] NULL,
	[CreateUser] [nvarchar](50) NULL,
	[CreateTime] [datetime] NULL,
	[ModifyUser] [nvarchar](50) NULL,
	[ModifyTime] [datetime] NULL,
	[State] [int] NULL,
 CONSTRAINT [PK_PCS_CRAFT_STATION_MEASURE] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PCS_Craft_Station_Material]    Script Date: 04/02/2018 17:52:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PCS_Craft_Station_Material](
	[UID] [int] IDENTITY(1,1) NOT NULL,
	[CraftID] [nvarchar](50) NULL,
	[WorkStationID] [nvarchar](50) NULL,
	[MaterialID] [nvarchar](50) NULL,
	[SingleConsumeQuantity] [money] NULL,
	[ShortWarningQuantity] [money] NULL,
	[CreateUser] [nvarchar](50) NULL,
	[CreateTime] [datetime] NULL,
	[ModifyUser] [nvarchar](50) NULL,
	[ModifyTime] [datetime] NULL,
	[State] [int] NULL,
 CONSTRAINT [PK_PCS_CRAFT_STATION_MATERIAL] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PCS_Craft_Station_Fixture]    Script Date: 04/02/2018 17:52:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PCS_Craft_Station_Fixture](
	[UID] [int] IDENTITY(1,1) NOT NULL,
	[CraftID] [nvarchar](50) NULL,
	[WorkStationID] [nvarchar](50) NULL,
	[FixtureType] [nvarchar](50) NULL,
	[FixtureOrder] [int] NULL,
	[CreateUser] [nvarchar](50) NULL,
	[CreateTime] [datetime] NULL,
	[ModifyUser] [nvarchar](50) NULL,
	[ModifyTime] [datetime] NULL,
	[State] [int] NULL,
 CONSTRAINT [PK_PCS_CRAFT_STATION_FIXTURE] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PCS_Craft_Station]    Script Date: 04/02/2018 17:52:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PCS_Craft_Station](
	[UID] [int] IDENTITY(1,1) NOT NULL,
	[CraftID] [nvarchar](50) NULL,
	[WorkStationID] [nvarchar](50) NULL,
	[IsEnabled] [int] NULL,
 CONSTRAINT [PK_PCS_CRAFT_STATION] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PCS_Craft]    Script Date: 04/02/2018 17:52:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PCS_Craft](
	[UID] [int] IDENTITY(1,1) NOT NULL,
	[WorkShopID] [nvarchar](50) NULL,
	[WorkLineID] [nvarchar](50) NULL,
	[CraftID] [nvarchar](50) NULL,
	[ProductID] [nvarchar](50) NULL,
	[ProductName] [nvarchar](50) NULL,
	[AssySerialRule] [int] NULL,
	[AssySerialInit] [int] NULL,
	[PackSerialRule] [int] NULL,
	[PackSerialInit] [int] NULL,
	[CreateUser] [nvarchar](50) NULL,
	[CreateTime] [datetime] NULL,
	[ModifyUser] [nvarchar](50) NULL,
	[ModifyTime] [datetime] NULL,
	[State] [int] NULL,
	[Description] [nvarchar](255) NULL,
 CONSTRAINT [PK_PCS_CRAFT] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PCS_Base_Worker]    Script Date: 04/02/2018 17:52:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PCS_Base_Worker](
	[UID] [int] IDENTITY(1,1) NOT NULL,
	[WorkShopID] [nvarchar](50) NULL,
	[WorkLineID] [nvarchar](50) NULL,
	[WorkerID] [nvarchar](50) NULL,
	[WorkerName] [nvarchar](50) NULL,
	[WorkerBarcode] [nvarchar](50) NULL,
	[WorkerDuty] [int] NULL,
	[CreateUser] [nvarchar](50) NULL,
	[CreateTime] [datetime] NULL,
	[ModifyUser] [nvarchar](50) NULL,
	[ModifyTime] [datetime] NULL,
	[State] [int] NULL,
	[Description] [nvarchar](255) NULL,
 CONSTRAINT [PK_PCS_BASE_WORKER] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PCS_Base_Station]    Script Date: 04/02/2018 17:52:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PCS_Base_Station](
	[UID] [int] IDENTITY(1,1) NOT NULL,
	[WorkShopID] [nvarchar](50) NULL,
	[WorkLineID] [nvarchar](50) NULL,
	[WorkStationID] [nvarchar](50) NULL,
	[WorkStationName] [nvarchar](50) NULL,
	[WorkStationCode] [nvarchar](50) NULL,
	[IsPackage] [int] NULL,
	[CreateUser] [nvarchar](50) NULL,
	[CreateTime] [datetime] NULL,
	[ModifyUser] [nvarchar](50) NULL,
	[ModifyTime] [datetime] NULL,
	[State] [int] NULL,
	[Description] [nvarchar](255) NULL,
 CONSTRAINT [PK_PCS_BASE_STATION] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PCS_Base_Measure]    Script Date: 04/02/2018 17:52:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PCS_Base_Measure](
	[UID] [int] IDENTITY(1,1) NOT NULL,
	[MeasureType] [nvarchar](50) NULL,
	[MeasureName] [nvarchar](50) NULL,
	[Unit] [nvarchar](50) NULL,
	[CreateUser] [nvarchar](50) NULL,
	[CreateTime] [datetime] NULL,
	[ModifyUser] [nvarchar](50) NULL,
	[ModifyTime] [datetime] NULL,
	[State] [int] NULL,
	[Description] [nvarchar](255) NULL,
 CONSTRAINT [PK_PCS_BASE_MEASURE] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PCS_Base_Material]    Script Date: 04/02/2018 17:52:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PCS_Base_Material](
	[UID] [int] IDENTITY(1,1) NOT NULL,
	[MaterialID] [nvarchar](50) NULL,
	[MaterialName] [nvarchar](50) NULL,
	[BarcodeRule] [nvarchar](50) NULL,
	[CreateUser] [nvarchar](50) NULL,
	[CreateTime] [datetime] NULL,
	[ModifyUser] [nvarchar](50) NULL,
	[ModifyTime] [datetime] NULL,
	[State] [int] NULL,
	[Description] [nvarchar](255) NULL,
 CONSTRAINT [PK_PCS_BASE_MATERIAL] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PCS_Base_Fixture]    Script Date: 04/02/2018 17:52:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PCS_Base_Fixture](
	[UID] [int] IDENTITY(1,1) NOT NULL,
	[FixtureID] [nvarchar](50) NULL,
	[FixtureName] [nvarchar](50) NULL,
	[FixtureType] [nvarchar](50) NULL,
	[FixtureBarcode] [nvarchar](50) NULL,
	[TotalCount] [int] NULL,
	[UsedCount] [int] NULL,
	[WarningCount] [int] NULL,
	[CreateUser] [nvarchar](50) NULL,
	[CreateTime] [datetime] NULL,
	[ModifyUser] [nvarchar](50) NULL,
	[ModifyTime] [datetime] NULL,
	[ScrapMarker] [int] NULL,
	[State] [int] NULL,
	[Description] [nvarchar](255) NULL,
 CONSTRAINT [PK_PCS_BASE_FIXTURE] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
