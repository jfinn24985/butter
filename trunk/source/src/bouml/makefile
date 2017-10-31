##START:version
#Butter version 1.2beta.175
##END:version
##START:date
#Tue Oct 31 13:03:23 2017
##END:date
##START:preamble
include $(ROOTDIR)/M_sys.mk

##END:preamble
##START:target bouml
BOUML_BOUMLSRC :=  \
	UmlBaseComponent.cpp \
	anItemKind.cpp \
	UmlBaseUseCaseView.cpp \
	UmlAttribute.cpp \
	UmlCom.cpp \
	UmlBaseDeploymentView.cpp \
	UmlExtraClassMember.cpp \
	UmlParameter.cpp \
	UmlCollaborationDiagram.cpp \
	UmlBaseItem.cpp \
	UmlBaseArtifact.cpp \
	UmlBaseClassView.cpp \
	IdlSettings.cpp \
	UmlBaseActualParameter.cpp \
	UmlSequenceDiagram.cpp \
	UmlNode.cpp \
	UmlDeploymentView.cpp \
	UmlBaseNcRelation.cpp \
	UmlSettingsCmd.cpp \
	UmlBaseDiagram.cpp \
	UmlBaseDeploymentDiagram.cpp \
	JavaSettings.cpp \
	UmlBaseFormalParameter.cpp \
	UmlUseCaseDiagram.cpp \
	UmlPackage.cpp \
	CmdFamily.cpp \
	UmlBaseExtraClassMember.cpp \
	UmlComponent.cpp \
	UmlClassMember.cpp \
	UmlBaseSequenceDiagram.cpp \
	UmlBaseRelation.cpp \
	UmlComponentView.cpp \
	UmlClassItem.cpp \
	CppSettingsCmd.cpp \
	UmlClass.cpp \
	OnInstanceCmd.cpp \
	MiscGlobalCmd.cpp \
	aDirection.cpp \
	UmlBaseClass.cpp \
	CppSettings.cpp \
	UmlBaseClassItem.cpp \
	UmlArtifact.cpp \
	aRelationKind.cpp \
	UmlBasePackage.cpp \
	UmlDiagram.cpp \
	UmlBuiltin.cpp \
	UmlBaseNode.cpp \
	UmlBaseAttribute.cpp \
	UmlRelation.cpp \
	UmlItem.cpp \
	JavaSettingsCmd.cpp \
	UmlSettings.cpp \
	UmlTypeSpec.cpp \
	UmlUseCase.cpp \
	UmlOperation.cpp \
	UmlClassView.cpp \
	ClassGlobalCmd.cpp \
	UmlUseCaseView.cpp \
	UmlDeploymentDiagram.cpp \
	UmlFormalParameter.cpp \
	aVisibility.cpp \
	UmlBaseDeployment.cpp \
	UmlBaseComponentView.cpp \
	UmlBaseComponentDiagram.cpp \
	UmlNcRelation.cpp \
	UmlStereotype.cpp \
	UmlBaseUseCase.cpp \
	UmlBaseCollaborationDiagram.cpp \
	IdlSettingsCmd.cpp \
	UmlClassDiagram.cpp \
	UmlBaseUseCaseDiagram.cpp \
	UmlComponentDiagram.cpp \
	PackageGlobalCmd.cpp \
	UmlActualParameter.cpp \
	UmlBaseOperation.cpp \
	UmlBaseClassMember.cpp \
	UmlBaseClassDiagram.cpp
BOUML_BOUML_DINC := $(filter %$(sufdep), $(BOUML_BOUMLSRC:.cpp=$(sufdep))) $(filter %$(sufdep), $(BOUML_BOUMLSRC:.c=$(sufdep)))
bouml$(suflib): LDFLAGS+= -L${QTDIR}/lib64 -lqt-mt -lm
bouml$(suflib) $(BOUML_BOUML_DINC) : CPPFLAGS+=-I${QTDIR}/include -DQT_NO_DEBUG -DQT_SHARED -DQT_THREAD_SUPPORT 
BOUML_BOUML_OBJ+=$(filter %$(sufobj), $(BOUML_BOUMLSRC:.cpp=$(sufobj)))
USEROBJ+=$(BOUML_BOUML_OBJ)
bouml$(suflib) : $(BOUML_BOUML_SRC) $(BOUML_BOUML_DINC)
-include $(BOUML_BOUML_DINC)
$(eval $(call do_archive,bouml$(suflib),$(BOUML_BOUML_OBJ)))

all :: bouml$(suflib)
TARGETS+=bouml$(suflib)

$(eval $(call do_install,bouml$(suflib),LIB))

##END:target bouml
